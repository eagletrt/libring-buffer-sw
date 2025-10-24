/*!
 * \file ring-buffer-api.h
 * \date 2025-03-29
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
 * \brief Initialize the buffer
 *
 * \details The buffer memory is allocated at runtime using the Arena Allocator
 * with a fixed capacity.
 * The \c cs_enter and \c cs_exit function callback are executed whenever there
 * are critical sections.
 *
 * \note The \c cs_enter and \c cs_exit callbacks can be NULL since they are
 * not mandatory for the buffer to work.
 *
 * \param[in] buffer The buffer handler structure
 * \param[in] data_size The size of a single item in bytes
 * \param[in] capacity The **maximum** number of items the buffer can handle simultaneously
 * \param[in] cs_enter The function callback used to enter a critical section
 * \param[in] cs_exit The function callback used to exit a critical section
 * \param[in] arena The arena allocator handler structure
 * \return RingBufferReturnCode
 *     - \c RING_BUFFER_RC_NULL if any parameter is \c NULL or on allocation failure
 *     - \c RING_BUFFER_RC_OK otherwise
 */
enum RingBufferReturnCode ring_buffer_api_init(
    struct RingBufferHandler *const buffer,
    const size_t data_size,
    const size_t capacity,
    void (*cs_enter)(void),
    void (*cs_exit)(void),
    ArenaAllocatorHandler_t *const arena);

/*!
 * \brief Check if the buffer is **empty**
 *
 * \param[in] buffer The buffer handler structure
 * \return \c true if the buffer is empty, \c false otherwise
 */
bool ring_buffer_api_is_empty(const struct RingBufferHandler *const buffer);

/*!
 * \brief Check if the buffer is **full**
 *
 * \param[in] buffer The buffer handler structure
 * \return \c true if the buffer is full, \c false otherwise
 */
bool ring_buffer_api_is_full(const struct RingBufferHandler *const buffer);

/*!
 * \brief Get the buffer **size**
 *
 * \details The buffer size is the current number of items stored inside it.
 * The maximum size is defined by the buffer capacity
 * 
 * \param[in] buffer The buffer handler structure
 * \return The buffer size
 */
size_t ring_buffer_api_size(const struct RingBufferHandler *const buffer);

/*!
 * \brief Get the buffer **capacity**
 *
 * \details The buffer capacity is the maximum number of items it can handle
 * simultaneously. The memory is allocated during initialization and cannot be
 * changed.
 * 
 * \param[in] buffer The buffer handler structure
 * \return The buffer capacity
 */
size_t ring_buffer_api_capacity(const struct RingBufferHandler *const buffer);

/*!
 * \brief Add an item at the front of the buffer
 *
 * \details If the buffer is not full the new item is inserted before the current
 * first element both changing the start index and the size of the buffer.
 *
 * \important Even if the item is passed by reference, its data is
 * **copied** into the buffer.
 *
 * \param[in] buffer The buffer handler structure
 * \param[in] item A **pointer** to the item to add
 * \return RingBufferReturnCode
 *     - \c RING_BUFFER_RC_NULL if any of the parameters are \c NULL
 *     - \c RING_BUFFER_RC_FULL if the buffer is full
 *     - \c RING_BUFFER_RC_OK otherwise
 */
enum RingBufferReturnCode ring_buffer_api_push_front(struct RingBufferHandler *const buffer, const void *const item);

/*!
 * \brief Add an item at the back of the buffer
 *
 * \details If the buffer is not full the new item is inserted after the last
 * element changing only the size of the buffer.
 *
 * \important Even if the item is passed by reference, its data is
 * **copied** into the buffer.
 *
 * \param[in] buffer The buffer handler structure
 * \param[in] item A **pointer** to the item to add
 * \return RingBufferReturnCode
 *     - \c RING_BUFFER_RC_NULL if any of the parameters are \c NULL
 *     - \c RING_BUFFER_RC_FULL if the buffer is full
 *     - \c RING_BUFFER_RC_OK otherwise
 */
enum RingBufferReturnCode ring_buffer_api_push_back(struct RingBufferHandler *const buffer, const void *const item);

/*!
 * \brief Remove an item from the front of the buffer
 *
 * \details If the buffer is not empty the first item is removed from the buffer
 * and its content is copied into the \c out variable.
 *
 * \note The \c out parameter can be \c NULL.
 *
 * \param[in] buffer The buffer handler structure
 * \param[out] out A reference to the memory area where the item is copied to
 * \return RingBufferReturnCode
 *     - \c RING_BUFFER_RC_NULL if any of the parameters are \c NULL
 *     - \c RING_BUFFER_RC_EMPTY if the buffer is empty
 *     - \c RING_BUFFER_RC_OK otherwise
 */
enum RingBufferReturnCode ring_buffer_api_pop_front(struct RingBufferHandler *const buffer, void *const out);

/*!
 * \brief Remove an item from the back of the buffer
 *
 * \details If the buffer is not empty the last item is removed from the buffer
 * and its content is copied into the \c out variable.
 *
 * \note The \c out parameter can be \c NULL.
 *
 * \param[in] buffer The buffer handler structure
 * \param[out] out A reference to the memory area where the item is copied to
 * \return RingBufferReturnCode
 *     - \c RING_BUFFER_RC_NULL if any of the parameters are \c NULL
 *     - \c RING_BUFFER_RC_EMPTY if the buffer is empty
 *     - \c RING_BUFFER_RC_OK otherwise
 */
enum RingBufferReturnCode ring_buffer_api_pop_back(struct RingBufferHandler *const buffer, void *const out);

/*!
 * \brief Get the item at the front of the buffer
 *
 * \details If the buffer is not empty the first element is copied into the \c out
 * variable.
 *
 * \note The \c out parameter can be \c NULL.
 * \important The item is not removed from the buffer.
 *
 * \param[in] buffer The buffer handler structure
 * \param[out] out A reference to the memory area where the item is copied to
 * \return RingBufferReturnCode
 *     - \c RING_BUFFER_RC_NULL if any of the parameters are \c NULL
 *     - \c RING_BUFFER_RC_EMPTY if the buffer is empty
 *     - \c RING_BUFFER_RC_OK otherwise
 */
enum RingBufferReturnCode ring_buffer_api_front(const struct RingBufferHandler *const buffer, void *const out);

/*!
 * \brief Get the item at the back of the buffer
 *
 * \details If the buffer is not empty the last element is copied into the \c out
 * variable.
 *
 * \note The \c out parameter can be \c NULL.
 * \important The item is not removed from the buffer.
 *
 * \param[in] buffer The buffer handler structure
 * \param[out] out A reference to the memory area where the item is copied to
 * \return RingBufferReturnCode
 *     - \c RING_BUFFER_RC_NULL if any of the parameters are \c NULL
 *     - \c RING_BUFFER_RC_EMPTY if the buffer is empty
 *     - \c RING_BUFFER_RC_OK otherwise
 */
enum RingBufferReturnCode ring_buffer_api_back(const struct RingBufferHandler *const buffer, void *out);

/*!
 * \brief Get a pointer to the front of the buffer
 *
 * \details Get the current reference of the first item of the buffer.
 *
 * \attention The content of the buffer may change so make sure to use the
 * reference **only when needed** to avoid access to an invalid item.
 *
 * \param[in] buffer The buffer handler structure
 * \return A reference to the front of the buffer
 */
void *ring_buffer_api_peek_front(const struct RingBufferHandler *const buffer);

/*!
 * \brief Get a pointer to the back of the buffer
 *
 * \details Get the current reference of the last item of the buffer.
 *
 * \attention The content of the buffer may change so make sure to use the
 * reference **only when needed** to avoid access to an invalid item.
 *
 * \param[in] buffer The buffer handler structure
 * \return A reference to the back of the buffer
 */
void *ring_buffer_api_peek_back(const struct RingBufferHandler *const buffer);

/*!
 * \brief Clear the buffer
 *
 * \details Remove all items inside the buffer
 *
 * \note The data is not actually removed and the array is not deallocated,
 * **only** the size and start of the array are modified
 *
 * \param[in] buffer The buffer handler structure
 * \return RingBufferReturnCode
 *     - \c RING_BUFFER_RC_NULL if any of the parameters are \c NULL
 *     - \c RING_BUFFER_RC_OK otherwise
 */
enum RingBufferReturnCode ring_buffer_api_clear(struct RingBufferHandler *const buffer);

#endif
