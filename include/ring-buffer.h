/*!
 * \file ring-buffer.h
 * \date 2026-03-14
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

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/*!
 * \brief Internal state handler for the ring buffer.
 */
struct RingBufferHandler {
    size_t start;           /*!< Index of the first element in the buffer */
    size_t size;            /*!< Current number of elements stored in the buffer */
    uint16_t data_size;     /*!< Size of a single data element in bytes */
    size_t capacity;        /*!< Maximum number of elements the buffer can hold */
    void (*cs_enter)(void); /*!< Optional callback to enter a critical section */
    void (*cs_exit)(void);  /*!< Optional callback to exit a critical section */
    void *data;             /*!< Pointer to the raw memory block where data is stored */
};
/*!
 * \brief All the possible return codes for the ring buffer functions
 */
enum RingBufferReturnCode {
    RING_BUFFER_RC_OK,           /*!< Operation completed successfully */
    RING_BUFFER_RC_NULL_POINTER, /*!< A provided pointer was NULL */
    RING_BUFFER_RC_EMPTY,        /*!< Buffer is empty; cannot pop elements */
    RING_BUFFER_RC_FULL          /*!< Buffer is full; cannot push elements */
};
2

#endif // RING_BUFFER_H