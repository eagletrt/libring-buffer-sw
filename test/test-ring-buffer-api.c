/*!
 * \file test-ring-buffer-api.c
 * \date 2025-03-29
 * \authors Antonio Gelain [antonio.gelain@studenti.unitn.it]
 * \authors Dorijan Di Zepp [dorijan.dizepp@eagletrt.it]
 *
 * \brief Library that implements a ring buffer using an arena allocator to
 *      dynamically allocate the buffer
 * 
 * \details A ring buffer, or circular buffer, is a fixed-size data structure 
 *      that wraps around when it reaches the end, allowing continuous reading 
 *      and writing without shifting elements. It is commonly used in real-time 
 *      systems, buffering data streams, and inter-process communication.
 * 
 * \warning The data buffer will not be deallocated automatically but has to be freed 
 *      by using the arena allocator.
 */

#include "unity.h"
#include "ring-buffer-api.h"

#include <stdio.h>

typedef struct {
    float x, y;
} Point;

RingBufferHandler_t int_buf;
RingBufferHandler_t point_buf;
ArenaAllocatorHandler_t arena;

void cs_enter(void) {
    printf("Entered\n");
}

void cs_exit(void) {
    printf("Exited\n");
}

void setUp(void) {
    arena_allocator_api_init(&arena);
    ring_buffer_api_init(&int_buf, sizeof(int), 10, NULL, NULL, &arena);
    ring_buffer_api_init(&point_buf, sizeof(Point), 10, NULL, NULL, &arena);
}

void tearDown(void) {
    ring_buffer_api_clear(&int_buf);
    ring_buffer_api_clear(&point_buf);
    arena_allocator_api_free(&arena);
}

/*! 
 * \defgroup ring_buffer_init Test ring buffer initialization
 * @{
 */

void check_ring_buffer_init_with_null(void) {
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_NULL_POINTER, ring_buffer_api_init(NULL, sizeof(float), 3, NULL, NULL, &arena));
}
void check_ring_buffer_init_return_value(void) {
    RingBufferHandler_t buf;
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_OK, ring_buffer_api_init(&buf, sizeof(float), 3, NULL, NULL, &arena));
}
void check_ring_buffer_init_defined_cs_function(void) {
    RingBufferHandler_t buf;
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_OK, ring_buffer_api_init(&buf, sizeof(float), 3, cs_enter, cs_exit, &arena));
}

/*! @} */

/*! 
 * \defgroup ring_buffer_empty Test ring buffer empty function
 * @{
 */

void check_ring_buffer_empty_with_null(void) {
    TEST_ASSERT_TRUE(ring_buffer_api_is_empty(NULL));
}
void check_ring_buffer_empty_when_empty(void) {
    TEST_ASSERT_TRUE(ring_buffer_api_is_empty(&int_buf));
}
void check_ring_buffer_empty_when_not_empty(void) {
    int_buf.size = 1;
    TEST_ASSERT_FALSE(ring_buffer_api_is_empty(&int_buf));
}

/*! @} */

/*! 
 * \defgroup ring_buffer_full Test ring buffer full function
 * @{
 */

void check_ring_buffer_full_with_null(void) {
    TEST_ASSERT_FALSE(ring_buffer_api_is_full(NULL));
}
void check_ring_buffer_full_when_full(void) {
    int_buf.size = int_buf.capacity;
    TEST_ASSERT_TRUE(ring_buffer_api_is_full(&int_buf));
}
void check_ring_buffer_full_when_not_full(void) {
    TEST_ASSERT_FALSE(ring_buffer_api_is_full(&int_buf));
}

/*! @} */

/*! 
 * \defgroup ring_buffer_size Test ring buffer size function
 * @{
 */

void check_ring_buffer_size_with_null(void) {
    TEST_ASSERT_EQUAL_size_t(0U, ring_buffer_api_size(NULL));
}
void check_ring_buffer_size(void) {
    const size_t size = 3;
    int_buf.size = size;
    TEST_ASSERT_EQUAL_size_t(size, ring_buffer_api_size(&int_buf));
}

/*! @} */

/*! 
 * \defgroup ring_buffer_push_front Test ring buffer push front function
 * @{
 */
void check_ring_buffer_push_front_with_null_handler(void) {
    Point p = { .x = 69.69f, .y = 2.7f };
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_NULL_POINTER, ring_buffer_api_push_front(NULL, &p));
}
void check_ring_buffer_push_front_with_null_item(void) {
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_NULL_POINTER, ring_buffer_api_push_front(&point_buf, NULL));
}
void check_ring_buffer_push_front_when_full(void) {
    Point p = { .x = 69.69f, .y = 2.7f };
    point_buf.size = point_buf.capacity;
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_FULL, ring_buffer_api_push_front(&point_buf, &p));
}
void check_ring_buffer_push_front_with_wrap_return_value(void) {
    Point p = { .x = 69.69f, .y = 2.7f };
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_OK, ring_buffer_api_push_front(&point_buf, &p));
}
void check_ring_buffer_push_front_without_wrap_return_value(void) {
    Point p = { .x = 69.69f, .y = 2.7f };
    point_buf.start = 2;
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_OK, ring_buffer_api_push_front(&point_buf, &p));
}
void check_ring_buffer_push_front_with_wrap_index(void) {
    Point p = { .x = 69.69f, .y = 2.7f };
    ring_buffer_api_push_front(&point_buf, &p);
    TEST_ASSERT_EQUAL_size_t(point_buf.capacity - 1, point_buf.start);
}
void check_ring_buffer_push_front_without_wrap_index(void) {
    Point p = { .x = 69.69f, .y = 2.7f };
    size_t index = 2;
    point_buf.start = index;
    ring_buffer_api_push_front(&point_buf, &p);
    TEST_ASSERT_EQUAL_size_t(index - 1, point_buf.start);
}
void check_ring_buffer_push_front_with_wrap_size(void) {
    Point p = { .x = 69.69f, .y = 2.7f };
    ring_buffer_api_push_front(&point_buf, &p);
    TEST_ASSERT_EQUAL_size_t(1, point_buf.size);
}
void check_ring_buffer_push_front_without_wrap_size(void) {
    Point p = { .x = 69.69f, .y = 2.7f };
    size_t index = 2;
    point_buf.start = index;
    ring_buffer_api_push_front(&point_buf, &p);
    TEST_ASSERT_EQUAL_size_t(1, point_buf.size);
}
void check_ring_buffer_push_front_with_wrap_data(void) {
    Point p = { .x = 69.69f, .y = 2.7f };
    ring_buffer_api_push_front(&point_buf, &p);
    TEST_ASSERT_NOT_EQUAL_INT_MESSAGE(&p, &((Point *)point_buf.data)[point_buf.capacity - 1], "The item in the buffer and the parameter share the same address");
    TEST_ASSERT_EQUAL_MEMORY(&p, &((Point *)point_buf.data)[point_buf.capacity - 1], sizeof(Point));
}
void check_ring_buffer_push_front_without_wrap_data(void) {
    Point p = { .x = 69.69f, .y = 2.7f };
    size_t index = 2;
    point_buf.start = index;
    ring_buffer_api_push_front(&point_buf, &p);
    TEST_ASSERT_NOT_EQUAL_INT_MESSAGE(&p, &((Point *)point_buf.data)[index - 1], "The item in the buffer and the parameter share the same address");
    TEST_ASSERT_EQUAL_MEMORY(&p, &((Point *)point_buf.data)[index - 1], sizeof(Point));
}

/*| @} */

/*! 
 * \defgroup ring_buffer_push_back Test ring buffer push back function
 * @{
 */

void check_ring_buffer_push_back_with_null_handler(void) {
    Point p = { .x = 69.69f, .y = 2.7f };
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_NULL_POINTER, ring_buffer_api_push_back(NULL, &p));
}
void check_ring_buffer_push_back_with_null_item(void) {
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_NULL_POINTER, ring_buffer_api_push_back(&point_buf, NULL));
}
void check_ring_buffer_push_back_when_full(void) {
    Point p = { .x = 69.69f, .y = 2.7f };
    point_buf.start = point_buf.capacity;
    point_buf.size = point_buf.capacity;
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_FULL, ring_buffer_api_push_back(&point_buf, &p));
}
void check_ring_buffer_push_back_with_wrap_return_value(void) {
    Point p = { .x = 69.69f, .y = 2.7f };
    point_buf.start = point_buf.capacity - 1;
    point_buf.size = 1;
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_OK, ring_buffer_api_push_back(&point_buf, &p));
}
void check_ring_buffer_push_back_without_wrap_return_value(void) {
    Point p = { .x = 69.69f, .y = 2.7f };
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_OK, ring_buffer_api_push_back(&point_buf, &p));
}
void check_ring_buffer_push_back_with_wrap_size(void) {
    Point p = { .x = 69.69f, .y = 2.7f };
    point_buf.start = point_buf.capacity - 1;
    point_buf.size = 1;
    ring_buffer_api_push_back(&point_buf, &p);
    TEST_ASSERT_EQUAL_size_t(2, point_buf.size);
}
void check_ring_buffer_push_back_without_wrap_size(void) {
    Point p = { .x = 69.69f, .y = 2.7f };
    ring_buffer_api_push_back(&point_buf, &p);
    TEST_ASSERT_EQUAL_size_t(1, point_buf.size);
}
void check_ring_buffer_push_back_with_wrap_data(void) {
    Point p = { .x = 69.69f, .y = 2.7f };
    point_buf.start = point_buf.capacity - 1;
    point_buf.size = 1;
    ring_buffer_api_push_back(&point_buf, &p);
    TEST_ASSERT_NOT_EQUAL_INT_MESSAGE(&p, &((Point *)point_buf.data)[0], "The item in the buffer and the parameter share the same address");
    TEST_ASSERT_EQUAL_MEMORY(&p, &((Point *)point_buf.data)[0], sizeof(Point));
}
void check_ring_buffer_push_back_without_wrap_data(void) {
    Point p = { .x = 69.69f, .y = 2.7f };
    ring_buffer_api_push_back(&point_buf, &p);
    TEST_ASSERT_NOT_EQUAL_INT_MESSAGE(&p, &((Point *)point_buf.data)[0], "The item in the buffer and the parameter share the same address");
    TEST_ASSERT_EQUAL_MEMORY(&p, &((Point *)point_buf.data)[0], sizeof(Point));
}

/*! @} */

/*! 
 * \defgroup ring_buffer_pop_front Test ring buffer pop front function
 * @{
 */

void check_ring_buffer_pop_front_with_null_handler(void) {
    Point p = { 0 };
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_NULL_POINTER, ring_buffer_api_pop_front(NULL, &p));
}
void check_ring_buffer_pop_front_with_null_item(void) {
    point_buf.size = 1;
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_OK, ring_buffer_api_pop_front(&point_buf, NULL));
}
void check_ring_buffer_pop_front_when_empty(void) {
    Point p = { 0 };
    point_buf.size = 0;
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_EMPTY, ring_buffer_api_pop_front(&point_buf, &p));
}
void check_ring_buffer_pop_front_with_wrap_return_value(void) {
    Point dot = { .x = 69.69f, .y = 2.7f };
    point_buf.start = point_buf.capacity - 1;
    point_buf.size = 1;
    ((Point *)point_buf.data)[point_buf.start] = dot;

    Point p = { 0 };
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_OK, ring_buffer_api_pop_front(&point_buf, &p));
}
void check_ring_buffer_pop_front_without_wrap_return_value(void) {
    Point dot = { .x = 69.69f, .y = 2.7f };
    point_buf.start = 0;
    point_buf.size = 1;
    ((Point *)point_buf.data)[point_buf.start] = dot;

    Point p = { 0 };
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_OK, ring_buffer_api_pop_front(&point_buf, &p));
}
void check_ring_buffer_pop_front_with_wrap_index(void) {
    Point dot = { .x = 69.69f, .y = 2.7f };
    point_buf.start = point_buf.capacity - 1;
    point_buf.size = 1;
    ((Point *)point_buf.data)[point_buf.start] = dot;

    Point p = { 0 };
    ring_buffer_api_pop_front(&point_buf, &p);
    TEST_ASSERT_EQUAL_size_t(0U, point_buf.start);
}
void check_ring_buffer_pop_front_without_wrap_index(void) {
    Point dot = { .x = 69.69f, .y = 2.7f };
    point_buf.start = 0;
    point_buf.size = 1;
    ((Point *)point_buf.data)[point_buf.start] = dot;

    Point p = { 0 };
    ring_buffer_api_pop_front(&point_buf, &p);
    TEST_ASSERT_EQUAL_size_t(1U, point_buf.start);
}
void check_ring_buffer_pop_front_with_wrap_size(void) {
    Point dot = { .x = 69.69f, .y = 2.7f };
    point_buf.start = point_buf.capacity - 1;
    point_buf.size = 1;
    ((Point *)point_buf.data)[point_buf.start] = dot;

    Point p = { 0 };
    ring_buffer_api_pop_front(&point_buf, &p);
    TEST_ASSERT_EQUAL_size_t(0U, point_buf.size);
}
void check_ring_buffer_pop_front_without_wrap_size(void) {
    Point dot = { .x = 69.69f, .y = 2.7f };
    point_buf.start = 0;
    point_buf.size = 1;
    ((Point *)point_buf.data)[point_buf.start] = dot;

    Point p = { 0 };
    ring_buffer_api_pop_front(&point_buf, &p);
    TEST_ASSERT_EQUAL_size_t(0U, point_buf.size);
}
void check_ring_buffer_pop_front_with_wrap_data(void) {
    Point dot = { .x = 69.69f, .y = 2.7f };
    point_buf.start = point_buf.capacity - 1;
    point_buf.size = 1;
    ((Point *)point_buf.data)[point_buf.start] = dot;

    Point p = { 0 };
    ring_buffer_api_pop_front(&point_buf, &p);
    TEST_ASSERT_NOT_EQUAL_INT_MESSAGE(&((Point *)point_buf.data)[point_buf.capacity - 1], &p, "The item in the buffer and the parameter share the same address");
    TEST_ASSERT_EQUAL_MEMORY(&dot, &p, sizeof(Point));
}
void check_ring_buffer_pop_front_without_wrap_data(void) {
    Point dot = { .x = 69.69f, .y = 2.7f };
    point_buf.start = 0;
    point_buf.size = 1;
    ((Point *)point_buf.data)[point_buf.start] = dot;

    Point p = { 0 };
    ring_buffer_api_pop_front(&point_buf, &p);
    TEST_ASSERT_NOT_EQUAL_INT_MESSAGE(&((Point *)point_buf.data)[point_buf.capacity - 1], &p, "The item in the buffer and the parameter share the same address");
    TEST_ASSERT_EQUAL_MEMORY(&dot, &p, sizeof(Point));
}

/*! @} */

/*! 
 * \defgroup ring_buffer_pop_back Test ring buffer pop back function
 * @{
 */

void check_ring_buffer_pop_back_with_null_handler(void) {
    Point p = { 0 };
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_NULL_POINTER, ring_buffer_api_pop_back(NULL, &p));
}
void check_ring_buffer_pop_back_with_null_item(void) {
    point_buf.size = 1;
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_OK, ring_buffer_api_pop_back(&point_buf, NULL));
}
void check_ring_buffer_pop_back_when_empty(void) {
    Point p = { 0 };
    point_buf.size = 0;
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_EMPTY, ring_buffer_api_pop_back(&point_buf, &p));
}
void check_ring_buffer_pop_back_with_wrap_return_value(void) {
    Point dot = { .x = 69.69f, .y = 2.7f };
    point_buf.start = point_buf.capacity - 1;
    point_buf.size = 2;
    ((Point *)point_buf.data)[0] = dot;

    Point p = { 0 };
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_OK, ring_buffer_api_pop_back(&point_buf, &p));
}
void check_ring_buffer_pop_back_without_wrap_return_value(void) {
    Point dot = { .x = 69.69f, .y = 2.7f };
    point_buf.start = 0;
    point_buf.size = 1;
    ((Point *)point_buf.data)[point_buf.start] = dot;

    Point p = { 0 };
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_OK, ring_buffer_api_pop_back(&point_buf, &p));
}
void check_ring_buffer_pop_back_with_wrap_size(void) {
    Point dot = { .x = 69.69f, .y = 2.7f };
    point_buf.start = point_buf.capacity - 1;
    point_buf.size = 2;
    ((Point *)point_buf.data)[0] = dot;

    Point p = { 0 };
    ring_buffer_api_pop_back(&point_buf, &p);
    TEST_ASSERT_EQUAL_size_t(1U, point_buf.size);
}
void check_ring_buffer_pop_back_without_wrap_size(void) {
    Point dot = { .x = 69.69f, .y = 2.7f };
    point_buf.start = 0;
    point_buf.size = 1;
    ((Point *)point_buf.data)[point_buf.start] = dot;

    Point p = { 0 };
    ring_buffer_api_pop_back(&point_buf, &p);
    TEST_ASSERT_EQUAL_size_t(0U, point_buf.size);
}
void check_ring_buffer_pop_back_with_wrap_data(void) {
    Point dot = { .x = 69.69f, .y = 2.7f };
    point_buf.start = point_buf.capacity - 1;
    point_buf.size = 2;
    ((Point *)point_buf.data)[0] = dot;

    Point p = { 0 };
    ring_buffer_api_pop_back(&point_buf, &p);
    TEST_ASSERT_NOT_EQUAL_INT_MESSAGE(&((Point *)point_buf.data)[0], &p, "The item in the buffer and the parameter share the same address");
    TEST_ASSERT_EQUAL_MEMORY(&dot, &p, sizeof(Point));
}
void check_ring_buffer_pop_back_without_wrap_data(void) {
    Point dot = { .x = 69.69f, .y = 2.7f };
    point_buf.start = 0;
    point_buf.size = 1;
    ((Point *)point_buf.data)[point_buf.start] = dot;

    Point p = { 0 };
    ring_buffer_api_pop_back(&point_buf, &p);
    TEST_ASSERT_NOT_EQUAL_INT_MESSAGE(&((Point *)point_buf.data)[0], &p, "The item in the buffer and the parameter share the same address");
    TEST_ASSERT_EQUAL_MEMORY(&dot, &p, sizeof(Point));
}

/*! @} */

/*! 
 * \defgroup ring_buffer_front Test ring buffer front function
 * @{
 */
void check_ring_buffer_front_with_null_handler(void) {
    Point p = { 0 };
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_NULL_POINTER, ring_buffer_api_front(NULL, &p));
}
void check_ring_buffer_front_with_null_item(void) {
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_NULL_POINTER, ring_buffer_api_front(&point_buf, NULL));
}
void check_ring_buffer_front_when_empty(void) {
    Point p = { 0 };
    point_buf.size = 0;
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_EMPTY, ring_buffer_api_front(&point_buf, &p));
}
void check_ring_buffer_front_when_not_empty_return_value(void) {
    Point dot = { .x = 69.69f, .y = 2.7f };
    point_buf.size = 1;
    ((Point *)point_buf.data)[0] = dot;

    Point p = { 0 };
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_OK, ring_buffer_api_front(&point_buf, &p));
}
void check_ring_buffer_front_when_not_empty_data(void) {
    Point dot = { .x = 69.69f, .y = 2.7f };
    point_buf.size = 1;
    ((Point *)point_buf.data)[0] = dot;

    Point p = { 0 };
    ring_buffer_api_front(&point_buf, &p);
    TEST_ASSERT_NOT_EQUAL_INT_MESSAGE(&((Point *)point_buf.data)[0], &p, "The item in the buffer and the parameter share the same address");
    TEST_ASSERT_EQUAL_MEMORY(&dot, &p, sizeof(Point));
}

/*! @} */

/*! 
 * \defgroup ring_buffer_back Test ring buffer back function
 * @{
 */
void check_ring_buffer_back_with_null_handler(void) {
    Point p = { 0 };
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_NULL_POINTER, ring_buffer_api_back(NULL, &p));
}
void check_ring_buffer_back_with_null_item(void) {
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_NULL_POINTER, ring_buffer_api_back(&point_buf, NULL));
}
void check_ring_buffer_back_when_empty(void) {
    Point p = { 0 };
    point_buf.size = 0;
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_EMPTY, ring_buffer_api_back(&point_buf, &p));
}
void check_ring_buffer_back_when_not_empty_return_value(void) {
    Point dot = { .x = 69.69f, .y = 2.7f };
    point_buf.size = 1;
    ((Point *)point_buf.data)[0] = dot;

    Point p = { 0 };
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_OK, ring_buffer_api_back(&point_buf, &p));
}
void check_ring_buffer_back_when_not_empty_data(void) {
    Point dot = { .x = 69.69f, .y = 2.7f };
    point_buf.size = 1;
    ((Point *)point_buf.data)[0] = dot;

    Point p = { 0 };
    ring_buffer_api_back(&point_buf, &p);
    TEST_ASSERT_NOT_EQUAL_INT_MESSAGE(&((Point *)point_buf.data)[0], &p, "The item in the buffer and the parameter share the same address");
    TEST_ASSERT_EQUAL_MEMORY(&dot, &p, sizeof(Point));
}
void check_ring_buffer_back_when_full_with_wrap_data(void) {
    Point dot = { .x = 69.69f, .y = 2.7f };
    point_buf.size = 1;
    ((Point *)point_buf.data)[0] = dot;

    Point p = { 0 };
    point_buf.start = 1;
    point_buf.capacity = 1;
    ring_buffer_api_back(&point_buf, &p);
    TEST_ASSERT_NOT_EQUAL_INT_MESSAGE(&((Point *)point_buf.data)[0], &p, "The item in the buffer and the parameter share the same address");
    TEST_ASSERT_EQUAL_MEMORY(&dot, &p, sizeof(Point));
}

/*! @} */

/*! 
 * \defgroup ring_buffer_peek_front Test ring buffer peek front
 * @{
 */

void check_ring_buffer_peek_front_with_null(void) {
    TEST_ASSERT_NULL(ring_buffer_api_peek_front(NULL));
}
void check_ring_buffer_peek_front_when_empty(void) {
    point_buf.size = 0;
    TEST_ASSERT_NULL(ring_buffer_api_peek_front(&point_buf));
}
void check_ring_buffer_peek_front_when_not_empty(void) {
    Point dot = { .x = 69.69f, .y = 2.7f };
    point_buf.size = 1;
    ((Point *)point_buf.data)[0] = dot;

    Point *p = (Point *)ring_buffer_api_peek_front(&point_buf);
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&((Point *)point_buf.data)[0], p, "Variable does not point to the right element in the buffer");
    TEST_ASSERT_EQUAL_MEMORY(&dot, p, sizeof(Point));
}

void check_ring_buffer_peek_back_with_null(void) {
    TEST_ASSERT_NULL(ring_buffer_api_peek_back(NULL));
}
void check_ring_buffer_peek_back_when_empty(void) {
    point_buf.size = 0;
    TEST_ASSERT_NULL(ring_buffer_api_peek_back(&point_buf));
}
void check_ring_buffer_peek_back_when_not_empty(void) {
    Point dot = { .x = 69.69f, .y = 2.7f };
    point_buf.size = 1;
    ((Point *)point_buf.data)[0] = dot;

    Point *p = (Point *)ring_buffer_api_peek_back(&point_buf);
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&((Point *)point_buf.data)[0], p, "Variable does not point to the right element in the buffer");
    TEST_ASSERT_EQUAL_MEMORY(&dot, p, sizeof(Point));
}
void check_ring_buffer_peek_back_when_full_with_wrap_data(void) {
    Point dot = { .x = 69.69f, .y = 2.7f };
    point_buf.size = 1;
    ((Point *)point_buf.data)[0] = dot;

    point_buf.start = 1;
    point_buf.capacity = 1;
    Point *p = (Point *)ring_buffer_api_peek_back(&point_buf);
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&((Point *)point_buf.data)[0], p, "Variable does not point to the right element in the buffer");
    TEST_ASSERT_EQUAL_MEMORY(&dot, p, sizeof(Point));
}

/*! @} */

/*! 
 * \defgroup ring_buffer_clear Test ring buffer clear function
 * @{
 */

void check_ring_buffer_clear_with_null_return_value(void) {
    const size_t start = 3;
    const size_t size = 4;
    int_buf.start = start;
    int_buf.size = size;
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_NULL_POINTER, ring_buffer_api_clear(NULL));
}
void check_ring_buffer_clear_with_null_start(void) {
    const size_t start = 3;
    const size_t size = 4;
    int_buf.start = start;
    int_buf.size = size;
    ring_buffer_api_clear(NULL);
    TEST_ASSERT_EQUAL_size_t(start, int_buf.start);
}
void check_ring_buffer_clear_with_null_size(void) {
    const size_t start = 3;
    const size_t size = 4;
    int_buf.start = start;
    int_buf.size = size;
    ring_buffer_api_clear(NULL);
    TEST_ASSERT_EQUAL_size_t(size, int_buf.size);
}
void check_ring_buffer_clear_return_value(void) {
    const size_t start = 3;
    const size_t size = 4;
    int_buf.start = start;
    int_buf.size = size;
    TEST_ASSERT_EQUAL_INT(RING_BUFFER_OK, ring_buffer_api_clear(&int_buf));
}
void check_ring_buffer_clear_start(void) {
    const size_t start = 3;
    const size_t size = 4;
    int_buf.start = start;
    int_buf.size = size;
    ring_buffer_api_clear(&int_buf);
    TEST_ASSERT_EQUAL_size_t(0U, int_buf.start);
}
void check_ring_buffer_clear_size(void) {
    const size_t start = 3;
    const size_t size = 4;
    int_buf.start = start;
    int_buf.size = size;
    ring_buffer_api_clear(&int_buf);
    TEST_ASSERT_EQUAL_size_t(0U, int_buf.size);
}

/*! @} */

int main() {
    UNITY_BEGIN();

    /*! 
     * \addtogroup ring_buffer_init Run test for ring buffer initialization
     * @{
     */

    RUN_TEST(check_ring_buffer_init_with_null);
    RUN_TEST(check_ring_buffer_init_return_value);
    RUN_TEST(check_ring_buffer_init_defined_cs_function);

    /*! @} */

    /*! 
     * \addtogroup ring_buffer_empty Run test for ring buffer empty function
     * @{
     */

    RUN_TEST(check_ring_buffer_empty_with_null);
    RUN_TEST(check_ring_buffer_empty_when_empty);
    RUN_TEST(check_ring_buffer_empty_when_not_empty);

    /*! @} */

    /*! 
     * \addtogroup ring_buffer_full Run test for ring buffer full function
     * @{
     */

    RUN_TEST(check_ring_buffer_full_with_null);
    RUN_TEST(check_ring_buffer_full_when_full);
    RUN_TEST(check_ring_buffer_full_when_not_full);

    /*! @} */

    /*! 
     * \addtogroup ring_buffer_size Run test for ring buffer size function
     * @{
     */

    RUN_TEST(check_ring_buffer_size_with_null);
    RUN_TEST(check_ring_buffer_size);

    /*! @} */

    /*! 
     * \addtogroup ring_buffer_push_front Run test for ring buffer push front function
     * @{
     */

    RUN_TEST(check_ring_buffer_push_front_with_null_handler);
    RUN_TEST(check_ring_buffer_push_front_with_null_item);
    RUN_TEST(check_ring_buffer_push_front_when_full);
    RUN_TEST(check_ring_buffer_push_front_with_wrap_return_value);
    RUN_TEST(check_ring_buffer_push_front_without_wrap_return_value);
    RUN_TEST(check_ring_buffer_push_front_with_wrap_index);
    RUN_TEST(check_ring_buffer_push_front_without_wrap_index);
    RUN_TEST(check_ring_buffer_push_front_with_wrap_size);
    RUN_TEST(check_ring_buffer_push_front_without_wrap_size);
    RUN_TEST(check_ring_buffer_push_front_with_wrap_data);
    RUN_TEST(check_ring_buffer_push_front_without_wrap_data);

    /*! @} */

    /*! 
     * \addtogroup ring_buffer_push_back Run test for ring buffer push back function
     * @{
     */

    RUN_TEST(check_ring_buffer_push_back_with_null_handler);
    RUN_TEST(check_ring_buffer_push_back_with_null_item);
    RUN_TEST(check_ring_buffer_push_back_when_full);
    RUN_TEST(check_ring_buffer_push_back_with_wrap_return_value);
    RUN_TEST(check_ring_buffer_push_back_without_wrap_return_value);
    RUN_TEST(check_ring_buffer_push_back_with_wrap_size);
    RUN_TEST(check_ring_buffer_push_back_without_wrap_size);
    RUN_TEST(check_ring_buffer_push_back_with_wrap_data);
    RUN_TEST(check_ring_buffer_push_back_without_wrap_data);

    /*! @} */

    /*! 
     * \addtogroup ring_buffer_pop_front Run test for ring buffer pop front function
     * @{
     */

    RUN_TEST(check_ring_buffer_pop_front_with_null_handler);
    RUN_TEST(check_ring_buffer_pop_front_with_null_item);
    RUN_TEST(check_ring_buffer_pop_front_when_empty);
    RUN_TEST(check_ring_buffer_pop_front_with_wrap_return_value);
    RUN_TEST(check_ring_buffer_pop_front_without_wrap_return_value);
    RUN_TEST(check_ring_buffer_pop_front_with_wrap_index);
    RUN_TEST(check_ring_buffer_pop_front_without_wrap_index);
    RUN_TEST(check_ring_buffer_pop_front_with_wrap_size);
    RUN_TEST(check_ring_buffer_pop_front_without_wrap_size);
    RUN_TEST(check_ring_buffer_pop_front_with_wrap_data);
    RUN_TEST(check_ring_buffer_pop_front_without_wrap_data);

    /*! @} */

    /*! 
     * \addtogroup ring_buffer_pop_back Run test for ring buffer pop back function
     * @{
     */

    RUN_TEST(check_ring_buffer_pop_back_with_null_handler);
    RUN_TEST(check_ring_buffer_pop_back_with_null_item);
    RUN_TEST(check_ring_buffer_pop_back_when_empty);
    RUN_TEST(check_ring_buffer_pop_back_with_wrap_return_value);
    RUN_TEST(check_ring_buffer_pop_back_without_wrap_return_value);
    RUN_TEST(check_ring_buffer_pop_back_with_wrap_size);
    RUN_TEST(check_ring_buffer_pop_back_without_wrap_size);
    RUN_TEST(check_ring_buffer_pop_back_with_wrap_data);
    RUN_TEST(check_ring_buffer_pop_back_without_wrap_data);

    /*! @} */

    /*! 
     * \addtogroup ring_buffer_front Run test for ring buffer front function
     * @{
     */

    RUN_TEST(check_ring_buffer_front_with_null_handler);
    RUN_TEST(check_ring_buffer_front_with_null_item);
    RUN_TEST(check_ring_buffer_front_when_empty);
    RUN_TEST(check_ring_buffer_front_when_not_empty_return_value);
    RUN_TEST(check_ring_buffer_front_when_not_empty_data);

    /*! @} */

    /*! 
     * \addtogroup ring_buffer_back Run test for ring buffer back function
     * @{
     */
    RUN_TEST(check_ring_buffer_back_with_null_handler);
    RUN_TEST(check_ring_buffer_back_with_null_item);
    RUN_TEST(check_ring_buffer_back_when_empty);
    RUN_TEST(check_ring_buffer_back_when_not_empty_return_value);
    RUN_TEST(check_ring_buffer_back_when_not_empty_data);
    RUN_TEST(check_ring_buffer_back_when_full_with_wrap_data);

    /*! @} */

    /*! 
     * \addtogroup ring_buffer_peek_front Run test for ring buffer peek front
     * @{
     */

    RUN_TEST(check_ring_buffer_peek_front_with_null);
    RUN_TEST(check_ring_buffer_peek_front_when_empty);
    RUN_TEST(check_ring_buffer_peek_front_when_not_empty);

    RUN_TEST(check_ring_buffer_peek_back_with_null);
    RUN_TEST(check_ring_buffer_peek_back_when_empty);
    RUN_TEST(check_ring_buffer_peek_back_when_not_empty);
    RUN_TEST(check_ring_buffer_peek_back_when_full_with_wrap_data);

    /*! @} */

    /*! 
     * \addtogroup ring_buffer_clear Run test for ring buffer clear function
     * @{
     */

    RUN_TEST(check_ring_buffer_clear_with_null_return_value);
    RUN_TEST(check_ring_buffer_clear_with_null_start);
    RUN_TEST(check_ring_buffer_clear_with_null_size);
    RUN_TEST(check_ring_buffer_clear_return_value);
    RUN_TEST(check_ring_buffer_clear_start);
    RUN_TEST(check_ring_buffer_clear_size);

    /*! @} */

    UNITY_END();
}