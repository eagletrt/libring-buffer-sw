# RING-BUFFER

A [ring buffer](https://en.wikipedia.org/wiki/Circular_buffer) (also called a *circular buffer*, or *cyclic buffer*) 
is a [FIFO](https://en.wikipedia.org/wiki/FIFO_(computing_and_electronics)) based data structure
(like a [queue](https://en.wikipedia.org/wiki/Queue_(abstract_data_type)) for example),
where the data can be pushed and pulled as if it is a data stream.

This library provide a simple enough implementation of a ring buffer with dynamic allocation using an arena allocator.

> [!NOTE]
> The buffer can be thought as an array starting from the left and ending to the right
> where the items can be pushed or popped from the *front* or the *back*, if pushed from the
> front the buffer grows to the left, otherwise if pushed to the back it grows to the right

## Dependencies

This library uses [ArenaAllocator](https://github.com/eagletrt/libarena-allocator-sw.git) for memory management. Make sure to initialize the allocator handler before the buffer initialization step.

## Usage

To create a ring buffer, first declare a variable using `RingBufferHandler_t`. In order to initialize it, an arena allocator is needed.\
For example:
```c
RingBufferHandler_t int_buf;
RingBufferHandler_t point_buf;
ArenaAllocatorHandler_t arena;

arena_allocator_api_init(&arena);
ring_buffer_init(&int_buf, sizeof(int), 10, NULL, NULL, &arena);
ring_buffer_init(&point_buf, sizeof(struct), 4, NULL, NULL, &arena);
```

To remove any trace of the buffer, the solely `ring_buffer_clear` function is not sufficient as it doesn't deallocate the data buffer.
To do so, `arena_allocator_api_free` will be used as follows:

```c
ring_buffer_clear(&int_buf);
ring_buffer_clear(&point_buf);
arena_allocator_api_free(&arena);
```

The `cs_enter` and `cs_exit` function have to be implemented by the user
and should define the start and the end of a critical section respectively. \
With critical section it is intended a block of code in which an interrupt can happen
causing undefined behaviour with the data modified inside the block.

> [!NOTE]
> `NULL` can be passed in place of the `cs_enter` and `cs_exit` functions, in that case
> the ring buffer is not guaranteed to always work in case of interrupts

Here is an example of an implementation to disable, enable and recover interrupts on a
STM32 microcontroller:
```c
uint32_t primask = 0;
void cs_enter(void) {
    primask = __get_PRIMASK();
    __disable_irq();
}
void cs_exit(void) {
    if (!primask)
        __enable_irq();
}
```

> [!WARNING]
> The example above works by disabling **ALL** the interrupts and by recovering
> the previous state afterwards, this should be used carefully

The `RingBufferReturnCode` enum is return by most of the functions of this library
and **should always be checked** before attempting other operations with the data structure.

## Examples

For more info check the [examples](./examples/) folder.
