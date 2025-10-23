/*!
 * \file ring-buffer-api.h
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

#ifndef RING_BUFFER_API_H
#define RING_BUFFER_API_H

#include "ring-buffer.h"
#include "arena-allocator-api.h"

/*!
 * \brief Initialize the buffer
 * \attention The type and capacity parameters must be the same as the ones
 * used in the structure declaration above
 *
 * \details If the ring_buffer_new macro is used this function is not needed
 *
 * \param buffer The buffer hanler structure
 * \param type The type of the items
 * \param capacity The maximum number of elements of the buffer
 * \param cs_enter A pointer to a function that should manage a critical section (can be NULL)
 * \param cs_exit A pointer to a function that should exit a critical section (can be NULL)
 * \param arena The arena allocator handler
 * \return RingBufferReturnCode
 *     - RING_BUFFER_NULL if the buffer handler is NULL
 *     - RING_BUFFER_OK otherwise
 */
enum RingBufferReturnCode ring_buffer_api_init(
    struct RingBufferHandler *const buffer,
    const size_t data_size,
    const size_t capacity,
    void (*cs_enter)(void),
    void (*cs_exit)(void),
    ArenaAllocatorHandler_t *const arena);

/*!
 * \brief Check if the buffer is empty
 *
 * \param buffer The buffer handler structure
 * \return True if the buffer is empty, false otherwise
 */
bool ring_buffer_api_is_empty(const struct RingBufferHandler *const buffer);

/*!
 * \brief Check if the buffer is full
 *
 * \param buffer The buffer handler structure
 * \return True if the buffer is full, false otherwise
 */
bool ring_buffer_api_is_full(const struct RingBufferHandler *const buffer);

/*!
 * \brief Get the current number of elements in the buffer
 * 
 * \param buffer The buffer handler structure
 * \return size_t The buffer size
 */
size_t ring_buffer_api_size(const struct RingBufferHandler *const buffer);

/*!
 * \brief Get the maximum number of elements the buffer can contain
 * 
 * \param buffer The buffer handler structure
 * \return size_t The buffer capacity
 */
size_t ring_buffer_api_capacity(const struct RingBufferHandler *const buffer);

/*!
 * \brief Insert an element af the start of the buffer
 *
 * \param buffer The buffer handler structure
 * \param item A pointer to the item to insert
 * \return RingBufferReturnCode
 *     - RING_BUFFER_NULL if the buffer handler or the item are NULL
 *     - RING_BUFFER_FULL if the buffer is full
 *     - RING_BUFFER_OK otherwise
 */
enum RingBufferReturnCode ring_buffer_api_push_front(struct RingBufferHandler *const buffer, const void *const item);

/*!
 * \brief Insert an element af the end of the buffer
 *
 * \param buffer The buffer handler structure
 * \param item A pointer to the item to insertdum
 * \return RingBufferReturnCode
 *     - RING_BUFFER_NULL if the buffer handler or the item are NULL
 *     - RING_BUFFER_FULL if the buffer is full
 *     - RING_BUFFER_OK otherwise
 */
enum RingBufferReturnCode ring_buffer_api_push_back(struct RingBufferHandler *const buffer, const void *const item);

/*!
 * \brief Remove an element from the front of the buffer
 * \details The 'out' parameter can be NULL
 *
 * \param buffer The buffer handler structure
 * \param out A pointer to a variable where the removed item is copied into
 * \return RingBufferReturnCode
 *     - RING_BUFFER_NULL if the buffer handler is NULL
 *     - RING_BUFFER_EMPTY if the buffer is empty
 *     - RING_BUFFER_OK otherwise
 */
enum RingBufferReturnCode ring_buffer_api_pop_front(struct RingBufferHandler *const buffer, void *const out);

/*!
 * \brief Remove an element from the end of the buffer
 * \details The 'out' parameter can be NULL
 *
 * \param buffer The buffer handler structure
 * \param out A pointer to a variable where the removed item is copied into
 * \return RingBufferReturnCode
 *     - RING_BUFFER_NULL if the buffer handler is NULL
 *     - RING_BUFFER_EMPTY if the buffer is empty
 *     - RING_BUFFER_OK otherwise
 */
enum RingBufferReturnCode ring_buffer_api_pop_back(struct RingBufferHandler *const buffer, void *const out);

/*!
 * \brief Get a copy of the element at the start of the buffer
 *
 * \param buffer The buffer handler structure
 * \param out A pointer to a variable where the item is copied into
 * \return RingBufferReturnCode
 *     - RING_BUFFER_NULL if the buffer handler or out are NULL
 *     - RING_BUFFER_EMPTY if the buffer is empty
 *     - RING_BUFFER_OK otherwise
 */
enum RingBufferReturnCode ring_buffer_api_front(const struct RingBufferHandler *const buffer, void *const out);

/*!
 * \brief Get a copy of the element at the end of the buffer
 *
 * \param buffer The buffer handler structure
 * \param out A pointer to a variable where the item is copied into
 * \return RingBufferReturnCode
 *     - RING_BUFFER_NULL if the buffer handler or out are NULL
 *     - RING_BUFFER_EMPTY if the buffer is empty
 *     - RING_BUFFER_OK otherwise
 */
enum RingBufferReturnCode ring_buffer_api_back(const struct RingBufferHandler *const buffer, void *out);

/*!
 * \brief Get a pointer to the element at the start of the buffer
 * \attention Keep in mind that the content of the item can change even if the
 * pointer don't
 *
 * \param buffer The buffer handler structure
 * \return void * The item at the start of the buffer
 */
void *ring_buffer_api_peek_front(const struct RingBufferHandler *const buffer);

/*!
 * \brief Get a pointer to the element at the end of the buffer
 * \attention Keep in mind that the content of the item can change even if the
 * pointer don't
 *
 * \param buffer The buffer handler structure
 * \return void * The item at the end of the buffer
 */
void *ring_buffer_api_peek_back(const struct RingBufferHandler *const buffer);

/*!
 * \brief Clear the buffer removing all items
 * \details The actual data is not erased, only the size is modified
 *
 * \param buffer The buffer handler structure
 * \return RingBufferReturnCode
 *     - RING_BUFFER_NULL if the buffer handler is NULL
 *     - RING_BUFFER_OK otherwise
 */
enum RingBufferReturnCode ring_buffer_api_clear(struct RingBufferHandler *const buffer);

#endif
