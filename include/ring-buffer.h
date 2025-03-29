/**
 * @file ring-buffer.h
 * @brief Implementation of a circular buffer without dinamyc allocation
 * and interrupt safe
 *
 * @date 13 Mar 2024
 * @author Antonio Gelain [antonio.gelain@studenti.unitn.it]
 */

 #ifndef RING_BUFFER_H
 #define RING_BUFFER_H
 
 #include <stddef.h>
 #include <stdint.h>
 #include <stdbool.h>
 
 /**
  * @brief Type definiton for the ring buffer handler structure
  * @details As an example you can declare and initialize a new ring buffer structure
  * that contains 10 integers as follows:
  *      RingBuffer(int, 10) buf = ring_buffer_new(int, 10, NULL, NULL);
  *
  * @param TYPE The data type of the items
  * @param CAPACITY The maximum number of elements of the buffer
  *
  * @param start The index of the first item in the buffer
  * @param size The current number of elements in the buffer
  * @param data_size The size of a single element in bytes
  * @param capacity The maximum number of elements that the buffer can contain
  * @param cs_enter A pointer to a function that should avoid interrupts
  * to happen inside the critical section
  * @param cs_exit A pointer to a function that should restore the normal functionalities
  * of the interrupts and mark the end of a critical section
  */
 #define RingBuffer(TYPE, CAPACITY) \
 struct { \
     size_t start; \
     size_t size; \
     uint16_t data_size; \
     size_t capacity; \
     void (* cs_enter)(void); \
     void (* cs_exit)(void); \
     TYPE *data; \
 }
 
 /**
  * @brief Ring buffer handler structure initialization
  * @attention The TYPE and CAPACITY parameters must be the same as the ones
  * used in the structure declaration above
  *
  * @details As an example you can declare and initialize a new ring buffer structure
  * that contains 10 integers as follows:
  *      RingBuffer(int, 10) buf = ring_buffer_new(int, 10, NULL, NULL);
  *
  * @details If the ring_buffer_init function is used this macro is not needed
  *
  * @param TYPE The data type of the items
  * @param CAPACITY The maximum number of elements of the buffer
  * @param CS_ENTER A pointer to a function that should manage a critical section (can be NULL)
  * @param CS_EXIT A pointer to a function that should exit a critical section (can be NULL)
  */
 #define ring_buffer_new(TYPE, CAPACITY, CS_ENTER, CS_EXIT) \
 { \
     .start = 0, \
     .size = 0, \
     .data_size = sizeof(TYPE), \
     .capacity = CAPACITY, \
     .cs_enter = CS_ENTER != NULL ? CS_ENTER : _ring_buffer_cs_dummy, \
     .cs_exit = CS_EXIT != NULL ? CS_EXIT : _ring_buffer_cs_dummy, \
     .data = NULL \
 }
 
 /**
  * @brief Structure definition used to pass the buffer handler as a function parameter
  * @attention This function should not be used directly
  */
 typedef struct {
     size_t start;
     size_t size;
     uint16_t data_size;
     size_t capacity;
     void (* cs_enter)(void);
     void (* cs_exit)(void);
     void * data;
 } RingBufferInterface;
 
 /**
  * @brief Enum with all the possible return codes for the ring buffer functions
  */
 typedef enum {
     RING_BUFFER_OK,
     RING_BUFFER_NULL_POINTER,
     RING_BUFFER_EMPTY,
     RING_BUFFER_FULL
 } RingBufferReturnCode;
 
 #endif  // RING_BUFFER_H
 
 