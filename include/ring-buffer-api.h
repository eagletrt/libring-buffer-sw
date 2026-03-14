/*!
 * \file ring-buffer-api.h
 * \date 2026-03-14
 * \authors Antonio Gelain [antonio.gelain@studenti.unitn.it]
 * \authors Dorijan Di Zepp [dorijan.dizepp@eagletrt.it]
 *
 * \brief Ring Buffer APIs functions
 * 
 * \details A ring buffer, or circular buffer, is a fixed-size data structure 
 *      that wraps around when it reaches the end, allowing continuous reading 
 *      and writing without shifting elements. It is commonly used in real-time 
 *      systems, buffering data streams, and inter-process communication.
 * 
 *      The "pointer" to the first item of the buffer is usually called head or
 *      **front**, meanwhile the "pointer" to the last element is usually called
 *      tail or **back**.
 *
 *      This ring buffer implementation allows to acces both the front and the
 *      back of the buffer as a **double-ended queue**.
 *
 * \attention The data buffer **will not** be deallocated automatically but has
 *      to be freed by using the arena allocator.
 */

#ifndef RING_BUFFER_API_H
#define RING_BUFFER_API_H

#include "ring-buffer.h"
#include "arena-allocator-api.h"

/*!
 * \brief Initializes the ring buffer structure and allocates memory.
 *
 * \details This function binds the buffer to an arena allocator and sets up 
 * optional callbacks.
 *
 * \param[in,out] buffer    Pointer to the the handler structure to initialize.
 * \param[in]     data_size Size of a single element in bytes (e.g., sizeof(int)).
 * \param[in]     capacity  Maximum number of elements the buffer can hold.
 * \param[in]     cs_enter  Pointer to function for entering a critical section (optional, can be NULL).
 * \param[in]     cs_exit   Pointer to function for exiting a critical section (optional, can be NULL).
 * \param[in,out] arena     Pointer to the ArenaAllocatorHandler used for memory allocation.
 * \retval RING_BUFFER_RC_NULL_POINTER If buffer or arena is NULL, or if memory allocation fails.
 * \retval RING_BUFFER_RC_OK           If the initialization was successful.
 */
enum RingBufferReturnCode ring_buffer_api_init(
    struct RingBufferHandler *buffer,
    size_t data_size,
    size_t capacity,
    void (*cs_enter)(void),
    void (*cs_exit)(void),
    struct ArenaAllocatorHandler *arena);

/*!
 * \brief Check if the buffer is **full**
 *
 * \param[in] buffer The buffer handler structure
 * \return True if the buffer is empty, false otherwise
 */
bool ring_buffer_api_is_empty(const struct RingBufferHandler *buffer);

/*!
 * \brief Get the buffer **size**
 *
 * \param[in] buffer The buffer handler structure
 * \return True if the buffer is full, false otherwise
 */
bool ring_buffer_api_is_full(const struct RingBufferHandler *buffer);

/*!
 * \brief Get the buffer **capacity**
 *
 * \details The buffer capacity is the maximum number of items it can handle
 * simultaneously. The memory is allocated during initialization and cannot be
 * changed.
 * 
 * \param[in] buffer The buffer handler structure
 * \return size_t The buffer size
 */
size_t ring_buffer_api_size(const struct RingBufferHandler *buffer);

/*!
 * \brief Insert an element at the start of the buffer
 *
 * \param[in,out] buffer The buffer handler structure
 * \param[in] item A pointer to the item to insert
 * \retval RING_BUFFER_RC_NULL_POINTER if the buffer handler or the item are NULL
 * \retval RING_BUFFER_RC_FULL if the buffer is full
 * \retval RING_BUFFER_RC_OK if the item has been has been inserted at the front of the buffer
 */
enum RingBufferReturnCode ring_buffer_api_push_front(struct RingBufferHandler *buffer, const void *item);

/*!
 * \brief Insert an element at the end of the buffer
 *
 * \param[in,out] buffer The buffer handler structure
 * \param[in] item A pointer to the item to insert
 * \retval RING_BUFFER_RC_NULL_POINTER if the buffer handler or the item are NULL
 * \retval RING_BUFFER_RC_FULL if the buffer is full
 * \retval RING_BUFFER_RC_OK if the item has been inserted at the back of the buffer
 */
enum RingBufferReturnCode ring_buffer_api_push_back(struct RingBufferHandler *buffer, const void *item);

/*!
 * \brief Remove an element from the front of the buffer
 *
 * \details The 'out' parameter can be NULL
 *
 * \param[in,out] buffer The buffer handler structure
 * \param[out] out A pointer to a variable where the removed item is copied into
 * \retval RING_BUFFER_RC_NULL_POINTER if the buffer handler is NULL
 * \retval RING_BUFFER_RC_EMPTY if the buffer is empty
 * \retval RING_BUFFER_RC_OK if the front element has been removed
 */
enum RingBufferReturnCode ring_buffer_api_pop_front(struct RingBufferHandler *buffer, void *out);

/*!
 * \brief Remove an element from the end of the buffer
 *
 * \details The 'out' parameter can be NULL
 *
 * \param[in,out] buffer The buffer handler structure
 * \param[out] out A pointer to a variable where the removed item is copied into
 * \retval RING_BUFFER_RC_NULL_POINTER if the buffer handler is NULL
 * \retval RING_BUFFER_RC_EMPTY if the buffer is empty
 * \retval RING_BUFFER_RC_OK if the back element has been removed
 */
enum RingBufferReturnCode ring_buffer_api_pop_back(struct RingBufferHandler *buffer, void *out);

/*!
 * \brief Get the item at the front of the buffer
 *
 * \details If the buffer is not empty the first element is copied into the \c out
 * variable.
 *
 * \details If the buffer is empty, no data is copied to the destination pointer.
 * 
 * \param[in] buffer The buffer handler structure
 * \param[out] out A pointer to a variable where the item is copied into
 * \retval RING_BUFFER_RC_NULL_POINTER if the buffer handler or out are NULL
 * \retval RING_BUFFER_RC_EMPTY if the buffer is empty
 * \retval RING_BUFFER_RC_OK if a copy of the first element has been created
 */
enum RingBufferReturnCode ring_buffer_api_front(const struct RingBufferHandler *buffer, void *out);

/*!
 * \brief Get the item at the back of the buffer
 *
 * \details If the buffer is not empty the last element is copied into the \c out
 * variable.
 *
 * \note The \c out parameter can be \c NULL.
 * \important The item is not removed from the buffer.
 *
 * \details If the buffer is empty, no data is copied to the destination pointer.
 * 
 * \param[in] buffer The buffer handler structure
 * \param[out] out A pointer to a variable where the item is copied into
 * \retval RING_BUFFER_RC_NULL_POINTER if the buffer handler or out are NULL
 * \retval RING_BUFFER_RC_EMPTY if the buffer is empty
 * \retval RING_BUFFER_RC_OK if a copy of the last element has been created
 */
enum RingBufferReturnCode ring_buffer_api_back(const struct RingBufferHandler *buffer, void *out);

/*!
 * \brief Get a pointer to the element at the start of the buffer
 *
 * \attention Keep in mind that the content of the item can change even if the
 * pointer doesn't
 *
 * \param[in] buffer The buffer handler structure
 * \return void * The item at the start of the buffer, or NULL if the buffer is empty
 */
void *ring_buffer_api_peek_front(const struct RingBufferHandler *buffer);

/*!
 * \brief Get a pointer to the element at the end of the buffer
 *
 * \attention Keep in mind that the content of the item can change even if the
 * pointer doesn't
 *
 * \param[in] buffer The buffer handler structure
 * \return void * The item at the end of the buffer, or NULL if the buffer is empty
 */
void *ring_buffer_api_peek_back(const struct RingBufferHandler *buffer);

/*!
 * \brief Resets the buffer to an empty state.
 *
 * \details This function performs a logical clear by resetting the internal 
 * indices and the size counter. Previously stored data remains in the buffer 
 * until but it is no longer accessible through the public APIs.
 *
 * \param[in,out] buffer Pointer to the buffer handler structure.
 * \retval RING_BUFFER_RC_NULL_POINTER if the buffer handler is NULL.
 * \retval RING_BUFFER_RC_OK           if the buffer was successfully reset.
 */
enum RingBufferReturnCode ring_buffer_api_clear(struct RingBufferHandler *buffer);

#endif
