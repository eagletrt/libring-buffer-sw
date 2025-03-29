/**
 * @file ring-buffer.h
 * @brief Implementation of a circular buffer without dinamyc allocation
 * and interrupt safe
 *
 * @date 13 Mar 2024
 * @author Antonio Gelain [antonio.gelain@studenti.unitn.it]
 */

 #ifndef RING_BUFFER_API_H
 #define RING_BUFFER_API_H
 
 #include "ring-buffer.h"
 #include "arena-allocator-api.h"
 
 /**
  * @brief Initialize the buffer
  * @attention The type and capacity parameters must be the same as the ones
  * used in the structure declaration above
  *
  * @details If the ring_buffer_new macro is used this function is not needed
  *
  * @param buffer The buffer hanler structure
  * @param type The type of the items
  * @param capacity The maximum number of elements of the buffer
  * @param cs_enter A pointer to a function that should manage a critical section (can be NULL)
  * @param cs_exit A pointer to a function that should exit a critical section (can be NULL)
  * @param arena The arena allocator handler
  * @return RingBufferReturnCode
  *     - RING_BUFFER_NULL if the buffer handler is NULL
  *     - RING_BUFFER_OK otherwise
  */
 #define ring_buffer_init(buffer, type, capacity, cs_enter, cs_exit, arena) _ring_buffer_init((RingBufferInterface *)(buffer), sizeof(type), capacity, cs_enter, cs_exit, (ArenaAllocatorHandler_t *)(arena))
 
 /**
  * @brief Check if the buffer is empty
  *
  * @param buffer The buffer handler structure
  * @return True if the buffer is empty, false otherwise
  */
 #define ring_buffer_is_empty(buffer) _ring_buffer_is_empty((RingBufferInterface *)(buffer))
 
 /**
  * @brief Check if the buffer is full
  *
  * @param buffer The buffer handler structure
  * @return True if the buffer is full, false otherwise
  */
 #define ring_buffer_is_full(buffer) _ring_buffer_is_full((RingBufferInterface *)(buffer))
 
 /**
  * @brief Get the current number of elements in the buffer
  * 
  * @param buffer The buffer handler structure
  * @return size_t The buffer size
  */
 #define ring_buffer_size(buffer) _ring_buffer_size((RingBufferInterface *)(buffer))
 
 /**
  * @brief Insert an element af the start of the buffer
  *
  * @param buffer The buffer handler structure
  * @param item A pointer to the item to insert
  * @return RingBufferReturnCode
  *     - RING_BUFFER_NULL if the buffer handler or the item are NULL
  *     - RING_BUFFER_FULL if the buffer is full
  *     - RING_BUFFER_OK otherwise
  */
 #define ring_buffer_push_front(buffer, item) _ring_buffer_push_front((RingBufferInterface *)(buffer), (void *)(item))
 
 /**
  * @brief Insert an element af the end of the buffer
  *
  * @param buffer The buffer handler structure
  * @param item A pointer to the item to insertdum
  * @return RingBufferReturnCode
  *     - RING_BUFFER_NULL if the buffer handler or the item are NULL
  *     - RING_BUFFER_FULL if the buffer is full
  *     - RING_BUFFER_OK otherwise
  */
 #define ring_buffer_push_back(buffer, item) _ring_buffer_push_back((RingBufferInterface *)(buffer), (void *)(item))
 
 /**
  * @brief Remove an element from the front of the buffer
  * @details The 'out' parameter can be NULL
  *
  * @param buffer The buffer handler structure
  * @param out A pointer to a variable where the removed item is copied into
  * @return RingBufferReturnCode
  *     - RING_BUFFER_NULL if the buffer handler is NULL
  *     - RING_BUFFER_EMPTY if the buffer is empty
  *     - RING_BUFFER_OK otherwise
  */
 #define ring_buffer_pop_front(buffer, out) _ring_buffer_pop_front((RingBufferInterface *)(buffer), (void *)(out))
 
 /**
  * @brief Remove an element from the end of the buffer
  * @details The 'out' parameter can be NULL
  *
  * @param buffer The buffer handler structure
  * @param out A pointer to a variable where the removed item is copied into
  * @return RingBufferReturnCode
  *     - RING_BUFFER_NULL if the buffer handler is NULL
  *     - RING_BUFFER_EMPTY if the buffer is empty
  *     - RING_BUFFER_OK otherwise
  */
 #define ring_buffer_pop_back(buffer, out) _ring_buffer_pop_back((RingBufferInterface *)(buffer), (void *)(out))
 
 /**
  * @brief Get a copy of the element at the start of the buffer
  *
  * @param buffer The buffer handler structure
  * @param out A pointer to a variable where the item is copied into
  * @return RingBufferReturnCode
  *     - RING_BUFFER_NULL if the buffer handler or out are NULL
  *     - RING_BUFFER_EMPTY if the buffer is empty
  *     - RING_BUFFER_OK otherwise
  */
 #define ring_buffer_front(buffer, out) _ring_buffer_front((RingBufferInterface *)(buffer), (void *)(out))
 
 /**
  * @brief Get a copy of the element at the end of the buffer
  *
  * @param buffer The buffer handler structure
  * @param out A pointer to a variable where the item is copied into
  * @return RingBufferReturnCode
  *     - RING_BUFFER_NULL if the buffer handler or out are NULL
  *     - RING_BUFFER_EMPTY if the buffer is empty
  *     - RING_BUFFER_OK otherwise
  */
 #define ring_buffer_back(buffer, out) _ring_buffer_back((RingBufferInterface *)(buffer), (void *)(out))
 
 /**
  * @brief Get a pointer to the element at the start of the buffer
  * @attention Keep in mind that the content of the item can change even if the
  * pointer don't
  *
  * @param buffer The buffer handler structure
  * @return void * The item at the start of the buffer
  */
 #define ring_buffer_peek_front(buffer) _ring_buffer_peek_front((RingBufferInterface *)(buffer))
 
 /**
  * @brief Get a pointer to the element at the end of the buffer
  * @attention Keep in mind that the content of the item can change even if the
  * pointer don't
  *
  * @param buffer The buffer handler structure
  * @return void * The item at the end of the buffer
  */
 #define ring_buffer_peek_back(buffer) _ring_buffer_peek_back((RingBufferInterface *)(buffer))
 
 /**
  * @brief Clear the buffer removing all items
  * @details The actual data is not erased, only the size is modified
  *
  * @param buffer The buffer handler structure
  * @return RingBufferReturnCode
  *     - RING_BUFFER_NULL if the buffer handler is NULL
  *     - RING_BUFFER_OK otherwise
  */
 #define ring_buffer_clear(buffer) _ring_buffer_clear((RingBufferInterface *)(buffer))
 
 /******************************************/
 /*   DO NOT USE THE FOLLOWING FUNCTIONS   */
 /*         USE THE MACRO INSTEAD          */
 /******************************************/
 
 RingBufferReturnCode _ring_buffer_init(
     RingBufferInterface * buffer,
     size_t data_size,
     size_t capacity,
     void (* cs_enter)(void),
     void (* cs_exit)(void),
     ArenaAllocatorHandler_t *arena
 );
 bool _ring_buffer_is_empty(RingBufferInterface * buffer);
 bool _ring_buffer_is_full(RingBufferInterface * buffer);
 size_t _ring_buffer_size(RingBufferInterface * buffer);
 RingBufferReturnCode _ring_buffer_push_front(RingBufferInterface * buffer, void * item);
 RingBufferReturnCode _ring_buffer_push_back(RingBufferInterface * buffer, void * item);
 RingBufferReturnCode _ring_buffer_pop_front(RingBufferInterface * buffer, void * out);
 RingBufferReturnCode _ring_buffer_pop_back(RingBufferInterface * buffer, void * out);
 RingBufferReturnCode _ring_buffer_front(RingBufferInterface * buffer, void * out);
 RingBufferReturnCode _ring_buffer_back(RingBufferInterface * buffer, void * out);
 void * _ring_buffer_peek_front(RingBufferInterface * buffer);
 void * _ring_buffer_peek_back(RingBufferInterface * buffer);
 RingBufferReturnCode _ring_buffer_clear(RingBufferInterface * buffer);
 
 // Function that substitute cs_enter and cs_exit if they are NULL
 void _ring_buffer_cs_dummy(void);
 
 #endif