/*!
 * \file ring-buffer.h
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

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/*!
 * \brief Structure definition used to pass the buffer handler as a function parameter
 * \attention This function should not be used directly
 */
typedef struct {
    size_t start;
    size_t size;
    uint16_t data_size;
    size_t capacity;
    void (*cs_enter)(void);
    void (*cs_exit)(void);
    void *data;
} RingBufferHandler_t;

/*!
 * \brief Enum with all the possible return codes for the ring buffer functions
 */
typedef enum {
    RING_BUFFER_OK,
    RING_BUFFER_NULL_POINTER,
    RING_BUFFER_EMPTY,
    RING_BUFFER_FULL
} RingBufferReturnCode;

#endif // RING_BUFFER_H