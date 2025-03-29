# RING-BUFFER

A [ring buffer](https://en.wikipedia.org/wiki/Circular_buffer) (also called a *circular buffer*, or *cyclic buffer*) 
is a [FIFO](https://en.wikipedia.org/wiki/FIFO_(computing_and_electronics)) based data structure
(like a [queue](https://en.wikipedia.org/wiki/Queue_(abstract_data_type)) for example),
where the data can be pushed and pulled as if it is a data stream.

This library provide a simple enough implementation of a ring buffer with dynamic allocation.

> [!NOTE]
> The buffer can be thought as an array starting from the left and ending to the right
> where the items can be pushed or popped from the *front* or the *back*, if pushed from the
> front the buffer grows to the left, otherwise if pushed to the back it grows to the right

## Dependencies

This library uses [ArenaAllocator](https://github.com/eagletrt/libarena-allocator-sw.git) for memory management. Make sure to initialize ArenaAllocatorHandler_t to use prev_errors array, as shown in the following section.

## Usage

To create a ring buffer first declare a variable using the `RingBuffer` macro
by providing the item type and the maximum number of element of the buffer. \
For example:
```c
RingBuffer(int, 10) int_buf = ...;
RingBuffer(double, 7) double_buf = ...;
RingBuffer(struct Point, 2000) point_buf = ...;
```

Then initialize the buffer using the `ring_buffer_new` macro that requires
the same item type and capacity given in the declaration.<br/>
The macro doesn not allocate memory for the data buffer, meaning that the use of an arena allocator for initializzation is required.
```c
ArenaAllocatorHandler_t arena;
arena_allocator_api_init(&arena);
... = ring_buffer_new(int, 10, cs_enter, cs_exit);
... = ring_buffer_new(double, 7, cs_enter, cs_exit);
... = ring_buffer_new(struct Point, 2000, cs_enter, cs_exit);

<buffer>.data = arena_allocator_api_calloc(&arena, data_size, capacity);
```

The following function can also be used to initialize the buffer. An initialized arena allocator is required to allocate memory for storing the buffer's data.
```c
ArenaAllocatorHandler_t arena;
arena_allocator_api_init(&arena);
ring_buffer_init(&int_buf, int, 10, cs_enter, cs_exit, &arena);
ring_buffer_init(&double_buf, double, 7, cs_enter, cs_exit), &arena;
ring_buffer_init(&point_buf, struct Point, 2000, cs_enter, cs_exit, &arena);
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
