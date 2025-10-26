/*!
 * \file ring-buffer.h
 * \date 2025-03-29
 * \authors Antonio Gelain [antonio.gelain@studenti.unitn.it]
 * \authors Dorijan Di Zepp [dorijan.dizepp@eagletrt.it]
 *
 * \brief Structures, Enumerations and various constants needed to use the Ring Buffer library
 * 
 * \details A ring buffer, or circular buffer, is a **fixed-size** data structure 
 *      that wraps around when it reaches the end, allowing continuous reading 
 *      and writing without shifting elements. It is commonly used in real-time 
 *      systems, buffering data streams, and inter-process communication.
 *
 *      This circular buffer implementation uses a heap-allocated array and uses
 *      the **index** of the first element and the **number of items** in the buffer to
 *      keep track of the data.
 *  
 * \attention The data buffer will not be deallocated automatically but has to be freed 
 *      by using the Arena Allocator.
 */

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/*!
 * \brief Handler structure of the Ring Buffer
 *
 * \details The \c cs_enter and \c cs_exit callback are needed in a context
 * where the buffer usage may cause race conditions.
 * This is often the case in embedded systems for the interrupts since they
 * change the control flow of the program and could modify the buffer while
 * it's being used.
 *
 * \attention This structure should not be used directly
 */
struct RingBufferHandler {
    size_t start;           /*!< Index of the first element in the buffer */
    size_t size;            /*!< Number of items in the buffer */
    uint16_t data_size;     /*!< Size of a single item in bytes */
    size_t capacity;        /*!< Maximum number of items the buffer can handle simultaneously */
    void (*cs_enter)(void); /*!< Callback function needed to enter a critical section */
    void (*cs_exit)(void);  /*!< Callback function needed to exit a critical section */
    void *data;             /*!< Pointer to the array of items of the buffer */
};

/*!
 * \brief Enum with all the possible return codes for the ring buffer functions
 */
enum RingBufferReturnCode {
    RING_BUFFER_RC_OK,           /*!< Function executed succesfully */
    RING_BUFFER_RC_NULL_POINTER, /*!< A NULL parameter was given */
    RING_BUFFER_RC_EMPTY,        /*!< The buffer is empty */
    RING_BUFFER_RC_FULL          /*!< The buffer is full */
};

#endif // RING_BUFFER_H
