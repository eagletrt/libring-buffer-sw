/*!
 * \file ring-buffer-api.c
 * \date 2026-03-18
 * \authors Antonio Gelain [antonio.gelain@studenti.unitn.it]
 * \authors Dorijan Di Zepp [dorijan.dizepp@eagletrt.it]
 *
 * \brief Ring Buffer APIs functions implementations
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

#include "eagletrt-api.h"
#include "ring-buffer-api.h"

#include <string.h>

/*!
 * \brief A "No-Operation" dummy function for critical sections.
 * \details This function is assigned to the cs_enter and cs_exit pointers
 * during initialization if the user provides NULL.
 */
EAGLETRT_STATIC void prv_ring_buffer_cs_dummy(void) {
    EAGLETRT_API_NOP();
}

enum RingBufferReturnCode ring_buffer_api_init(
    struct RingBufferHandler *const buffer,
    const size_t data_size,
    const size_t capacity,
    void (*cs_enter)(void),
    void (*cs_exit)(void),
    struct ArenaAllocatorHandler *const arena) {
    if (buffer == NULL || arena == NULL) {
        return RING_BUFFER_RC_NULL_POINTER;
    }
    buffer->start = 0;
    buffer->size = 0;
    buffer->data_size = data_size;
    buffer->capacity = capacity;
    buffer->cs_enter = cs_enter != NULL ? cs_enter : prv_ring_buffer_cs_dummy;
    buffer->cs_exit = cs_exit != NULL ? cs_exit : prv_ring_buffer_cs_dummy;
    buffer->data = arena_allocator_api_calloc(arena, data_size, capacity);
    if (buffer->data == NULL) {
        return RING_BUFFER_RC_NULL_POINTER;
    }
    return RING_BUFFER_RC_OK;
}

bool ring_buffer_api_is_empty(const struct RingBufferHandler *const buffer) {
    if (buffer == NULL) {
        return true;
    }
    return buffer->size == 0;
}

bool ring_buffer_api_is_full(const struct RingBufferHandler *const buffer) {
    if (buffer == NULL) {
        return false;
    }
    return buffer->size >= buffer->capacity;
}

size_t ring_buffer_api_size(const struct RingBufferHandler *const buffer) {
    if (buffer == NULL) {
        return 0U;
    }
    return buffer->size;
}

size_t ring_buffer_api_capacity(const struct RingBufferHandler *const buffer) {
    if (buffer == NULL) {
        return 0U;
    }
    return buffer->capacity;
}

enum RingBufferReturnCode ring_buffer_api_push_front(struct RingBufferHandler *const buffer, const void *const item) {
    if (buffer == NULL || item == NULL) {
        return RING_BUFFER_RC_NULL_POINTER;
    }
    buffer->cs_enter();

    if (buffer->size >= buffer->capacity) {
        buffer->cs_exit();
        return RING_BUFFER_RC_FULL;
    }

    // Calculate index of the item in the buffer
    if (buffer->start == 0) {
        buffer->start = buffer->capacity;
    }
    --buffer->start;
    ++buffer->size;

    // Push item in the buffer
    const size_t data_size = buffer->data_size;
    uint8_t *base = (uint8_t *)buffer->data;
    memcpy(base + buffer->start * data_size, item, data_size);
    buffer->cs_exit();
    return RING_BUFFER_RC_OK;
}

enum RingBufferReturnCode ring_buffer_api_push_back(struct RingBufferHandler *const buffer, const void *const item) {
    if (buffer == NULL || item == NULL) {
        return RING_BUFFER_RC_NULL_POINTER;
    }

    buffer->cs_enter();

    if (buffer->size >= buffer->capacity) {
        buffer->cs_exit();
        return RING_BUFFER_RC_FULL;
    }

    // Calculate index of the item in the buffer
    size_t cur = buffer->start + buffer->size;
    if (cur >= buffer->capacity) {
        cur -= buffer->capacity;
    }

    // Push item in the buffer
    const size_t data_size = buffer->data_size;
    uint8_t *base = (uint8_t *)buffer->data;
    memcpy(base + cur * data_size, item, data_size);
    ++buffer->size;

    buffer->cs_exit();
    return RING_BUFFER_RC_OK;
}

enum RingBufferReturnCode ring_buffer_api_pop_front(struct RingBufferHandler *const buffer, void *const out) {
    if (buffer == NULL) {
        return RING_BUFFER_RC_NULL_POINTER;
    }
    buffer->cs_enter();

    if (buffer->size == 0) {
        buffer->cs_exit();
        return RING_BUFFER_RC_EMPTY;
    }

    // Pop the item from the buffer
    if (out != NULL) {
        const size_t data_size = buffer->data_size;
        uint8_t *base = (uint8_t *)buffer->data;
        memcpy(out, base + buffer->start * data_size, data_size);
    }

    // Update start and size
    ++buffer->start;
    if (buffer->start >= buffer->capacity) {
        buffer->start = 0;
    }
    --buffer->size;

    buffer->cs_exit();
    return RING_BUFFER_RC_OK;
}

enum RingBufferReturnCode ring_buffer_api_pop_back(struct RingBufferHandler *const buffer, void *const out) {
    if (buffer == NULL) {
        return RING_BUFFER_RC_NULL_POINTER;
    }
    buffer->cs_enter();

    if (buffer->size == 0) {
        buffer->cs_exit();
        return RING_BUFFER_RC_EMPTY;
    }

    // Pop the item from the buffer
    if (out != NULL) {
        size_t cur = buffer->start + buffer->size - 1;
        if (cur >= buffer->capacity) {
            cur -= buffer->capacity;
        }
        const size_t data_size = buffer->data_size;
        uint8_t *base = (uint8_t *)buffer->data;
        memcpy(out, base + cur * data_size, data_size);
    }
    --buffer->size;

    buffer->cs_exit();
    return RING_BUFFER_RC_OK;
}

enum RingBufferReturnCode ring_buffer_api_front(const struct RingBufferHandler *const buffer, void *const out) {
    if (buffer == NULL || out == NULL) {
        return RING_BUFFER_RC_NULL_POINTER;
    }
    buffer->cs_enter();

    if (buffer->size == 0) {
        buffer->cs_exit();
        return RING_BUFFER_RC_EMPTY;
    }

    // Copy data
    const size_t data_size = buffer->data_size;
    uint8_t *base = (uint8_t *)buffer->data;
    memcpy(out, base + buffer->start * data_size, data_size);

    buffer->cs_exit();
    return RING_BUFFER_RC_OK;
}

enum RingBufferReturnCode ring_buffer_api_back(const struct RingBufferHandler *const buffer, void *const out) {
    if (buffer == NULL || out == NULL) {
        return RING_BUFFER_RC_NULL_POINTER;
    }
    buffer->cs_enter();

    if (buffer->size == 0) {
        buffer->cs_exit();
        return RING_BUFFER_RC_EMPTY;
    }

    // Copy data
    size_t cur = buffer->start + buffer->size - 1;
    if (cur >= buffer->capacity) {
        cur -= buffer->capacity;
    }
    const size_t data_size = buffer->data_size;
    uint8_t *base = (uint8_t *)buffer->data;
    memcpy(out, base + cur * data_size, data_size);

    buffer->cs_exit();
    return RING_BUFFER_RC_OK;
}

void *ring_buffer_api_peek_front(const struct RingBufferHandler *const buffer) {
    if (buffer == NULL) {
        return NULL;
    }

    buffer->cs_enter();

    if (buffer->size == 0) {
        buffer->cs_exit();
        return NULL;
    }
    uint8_t *front = (uint8_t *)buffer->data + buffer->start * buffer->data_size;

    buffer->cs_exit();
    return front;
}

void *ring_buffer_api_peek_back(const struct RingBufferHandler *const buffer) {
    if (buffer == NULL) {
        return NULL;
    }
    buffer->cs_enter();

    if (buffer->size == 0) {
        buffer->cs_exit();
        return NULL;
    }

    // Calculate index of the element in the buffer
    size_t cur = buffer->start + buffer->size - 1;
    if (cur >= buffer->capacity) {
        cur -= buffer->capacity;
    }
    uint8_t *back = (uint8_t *)buffer->data + cur * buffer->data_size;

    buffer->cs_exit();
    return back;
}

enum RingBufferReturnCode ring_buffer_api_clear(struct RingBufferHandler *const buffer) {
    if (buffer == NULL) {
        return RING_BUFFER_RC_NULL_POINTER;
    }
    buffer->cs_enter();
    buffer->start = 0;
    buffer->size = 0;
    buffer->cs_exit();
    return RING_BUFFER_RC_OK;
}
