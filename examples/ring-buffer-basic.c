/*!
 * \file ring-buffer-basic.c
 * \date 2025-03-29
 * \authors Antonio Gelain [antonio.gelain2@gmail.com]
 * \authors Dorijan Di Zepp [dorijan.dizepp@eagletrt.it]
 *
 * \brief Simple example of a ring buffer that stores integer values.
 * \details In this example, we allocate both the arena allocator and the ring buffer  
 *      and insert some random numbers into it.  
 *      We then check whether the first inserted value is actually at the end of the  
 *      buffer and if the last one is at the start of the buffer.
 *      It can be verified by also printing all the values stored in it.  
 *      At the end of the program, all allocated memory is freed to prevent  
 *      memory leaks.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ring-buffer-api.h"

int main(void) {
    srand(time(NULL));
    ArenaAllocatorHandler_t arena;
    RingBufferHandler_t int_buf;

    arena_allocator_api_init(&arena);
    ring_buffer_api_init(&int_buf, sizeof(int), 10, NULL, NULL, &arena);

    // Push items in the buffer
    for (int i = 0; i < 5; ++i) {
        int num = rand() % 100 + 1;
        if (ring_buffer_api_push_back(&int_buf, &num) != RING_BUFFER_OK)
            printf("[ERROR]: Cant push element inside the buffer\n");
    }

    // Get items and info about the buffer
    printf("Buffer size: %lu\n", ring_buffer_api_size(&int_buf));
    int val = 0;
    if (ring_buffer_api_front(&int_buf, &val) == RING_BUFFER_OK)
        printf("Front element: %d\n", val);
    int *p_val = ring_buffer_api_peek_back(&int_buf);
    if (p_val != NULL)
        printf("Back element: %d\n", *p_val);

    // Remove items from the buffer
    printf("Values: ");
    while (!ring_buffer_api_is_empty(&int_buf)) {
        if (ring_buffer_api_pop_back(&int_buf, &val) == RING_BUFFER_OK)
            printf("%d ", val);
    }
    printf("\n");

    // Clear the buffer
    if (ring_buffer_api_clear(&int_buf) != RING_BUFFER_OK)
        printf("[ERROR]: Cant clear buffer\n");

    arena_allocator_api_free(&arena);

    return 0;
}