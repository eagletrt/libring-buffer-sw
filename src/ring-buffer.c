/*!
 * \file ring-buffer.c
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

#include "ring-buffer-api.h"

#include <string.h>

void ring_buffer_cs_dummy(void) {
}

RingBufferReturnCode ring_buffer_init(
    RingBufferHnadler_t *buffer,
    size_t data_size,
    size_t capacity,
    void (*cs_enter)(void),
    void (*cs_exit)(void),
    ArenaAllocatorHandler_t *arena) {
    if (buffer == NULL || arena == NULL)
        return RING_BUFFER_NULL_POINTER;
    buffer->start = 0;
    buffer->size = 0;
    buffer->data_size = data_size;
    buffer->capacity = capacity;
    buffer->cs_enter = cs_enter != NULL ? cs_enter : ring_buffer_cs_dummy;
    buffer->cs_exit = cs_exit != NULL ? cs_exit : ring_buffer_cs_dummy;
    buffer->data = arena_allocator_api_calloc(arena, data_size, capacity);
    if (buffer->data == NULL)
        return RING_BUFFER_NULL_POINTER;
    return RING_BUFFER_OK;
}

bool ring_buffer_is_empty(RingBufferHnadler_t *buffer) {
    if (buffer == NULL)
        return true;
    return buffer->size == 0;
}

bool ring_buffer_is_full(RingBufferHnadler_t *buffer) {
    if (buffer == NULL)
        return false;
    return buffer->size >= buffer->capacity;
}

size_t ring_buffer_size(RingBufferHnadler_t *buffer) {
    if (buffer == NULL)
        return 0U;
    return buffer->size;
}

RingBufferReturnCode ring_buffer_push_front(RingBufferHnadler_t *buffer, void *item) {
    if (buffer == NULL || item == NULL)
        return RING_BUFFER_NULL_POINTER;

    buffer->cs_enter();

    if (buffer->size >= buffer->capacity) {
        buffer->cs_exit();
        return RING_BUFFER_FULL;
    }

    // Calculate index of the item in the buffer
    if (buffer->start == 0)
        buffer->start = buffer->capacity;
    --buffer->start;
    ++buffer->size;

    // Push item in the buffer
    const size_t data_size = buffer->data_size;
    uint8_t *base = (uint8_t *)buffer->data;
    memcpy(base + buffer->start * data_size, item, data_size);

    buffer->cs_exit();
    return RING_BUFFER_OK;
}

RingBufferReturnCode ring_buffer_push_back(RingBufferHnadler_t *buffer, void *item) {
    if (buffer == NULL || item == NULL)
        return RING_BUFFER_NULL_POINTER;

    buffer->cs_enter();

    if (buffer->size >= buffer->capacity) {
        buffer->cs_exit();
        return RING_BUFFER_FULL;
    }

    // Calculate index of the item in the buffer
    size_t cur = buffer->start + buffer->size;
    if (cur >= buffer->capacity)
        cur -= buffer->capacity;

    // Push item in the buffer
    const size_t data_size = buffer->data_size;
    uint8_t *base = (uint8_t *)buffer->data;
    memcpy(base + cur * data_size, item, data_size);
    ++buffer->size;

    buffer->cs_exit();
    return RING_BUFFER_OK;
}

RingBufferReturnCode ring_buffer_pop_front(RingBufferHnadler_t *buffer, void *out) {
    if (buffer == NULL)
        return RING_BUFFER_NULL_POINTER;

    buffer->cs_enter();

    if (buffer->size == 0) {
        buffer->cs_exit();
        return RING_BUFFER_EMPTY;
    }

    // Pop the item from the buffer
    if (out != NULL) {
        const size_t data_size = buffer->data_size;
        uint8_t *base = (uint8_t *)buffer->data;
        memcpy(out, base + buffer->start * data_size, data_size);
    }

    // Update start and size
    ++buffer->start;
    if (buffer->start >= buffer->capacity)
        buffer->start = 0;
    --buffer->size;

    buffer->cs_exit();
    return RING_BUFFER_OK;
}

RingBufferReturnCode ring_buffer_pop_back(RingBufferHnadler_t *buffer, void *out) {
    if (buffer == NULL)
        return RING_BUFFER_NULL_POINTER;

    buffer->cs_enter();

    if (buffer->size == 0) {
        buffer->cs_exit();
        return RING_BUFFER_EMPTY;
    }

    // Pop the item from the buffer
    if (out != NULL) {
        size_t cur = buffer->start + buffer->size - 1;
        if (cur >= buffer->capacity)
            cur -= buffer->capacity;
        const size_t data_size = buffer->data_size;
        uint8_t *base = (uint8_t *)buffer->data;
        memcpy(out, base + cur * data_size, data_size);
    }
    --buffer->size;

    buffer->cs_exit();
    return RING_BUFFER_OK;
}

RingBufferReturnCode ring_buffer_front(RingBufferHnadler_t *buffer, void *out) {
    if (buffer == NULL || out == NULL)
        return RING_BUFFER_NULL_POINTER;

    buffer->cs_enter();

    if (buffer->size == 0) {
        buffer->cs_exit();
        return RING_BUFFER_EMPTY;
    }

    // Copy data
    const size_t data_size = buffer->data_size;
    uint8_t *base = (uint8_t *)buffer->data;
    memcpy(out, base + buffer->start * data_size, data_size);

    buffer->cs_exit();
    return RING_BUFFER_OK;
}

RingBufferReturnCode ring_buffer_back(RingBufferHnadler_t *buffer, void *out) {
    if (buffer == NULL || out == NULL)
        return RING_BUFFER_NULL_POINTER;

    buffer->cs_enter();

    if (buffer->size == 0) {
        buffer->cs_exit();
        return RING_BUFFER_EMPTY;
    }

    // Copy data
    size_t cur = buffer->start + buffer->size - 1;
    if (cur >= buffer->capacity)
        cur -= buffer->capacity;
    const size_t data_size = buffer->data_size;
    uint8_t *base = (uint8_t *)buffer->data;
    memcpy(out, base + cur * data_size, data_size);

    buffer->cs_exit();
    return RING_BUFFER_OK;
}

void *ring_buffer_peek_front(RingBufferHnadler_t *buffer) {
    if (buffer == NULL)
        return NULL;

    buffer->cs_enter();

    if (buffer->size == 0) {
        buffer->cs_exit();
        return NULL;
    }
    uint8_t *front = (uint8_t *)buffer->data + buffer->start * buffer->data_size;

    buffer->cs_exit();
    return front;
}

void *ring_buffer_peek_back(RingBufferHnadler_t *buffer) {
    if (buffer == NULL)
        return NULL;

    buffer->cs_enter();

    if (buffer->size == 0) {
        buffer->cs_exit();
        return NULL;
    }

    // Calculate index of the element in the buffer
    size_t cur = buffer->start + buffer->size - 1;
    if (cur >= buffer->capacity)
        cur -= buffer->capacity;
    uint8_t *back = (uint8_t *)buffer->data + cur * buffer->data_size;

    buffer->cs_exit();
    return back;
}

RingBufferReturnCode ring_buffer_clear(RingBufferHnadler_t *buffer) {
    if (buffer == NULL)
        return RING_BUFFER_NULL_POINTER;
    buffer->cs_enter();
    buffer->start = 0;
    buffer->size = 0;
    buffer->cs_exit();
    return RING_BUFFER_OK;
}