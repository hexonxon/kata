#include <assert.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

struct ringbuffer
{
    char *buffer;
    size_t capacity;
    size_t size;
    size_t head;
    size_t tail;
};

int rb_init(struct ringbuffer *rb, size_t size)
{
    if (!rb || !size) {
        return -EINVAL;
    }

    rb->buffer = malloc(size);
    rb->capacity = size;
    rb->size = rb->head = rb->tail = 0;

    return 0;
}

int rb_enqueue(struct ringbuffer *rb, const char* data, size_t size)
{
    if (!rb || !rb->buffer) {
        return -EINVAL;
    }

    if (!data) {
        return -EINVAL;
    }

    size_t avail = rb->capacity - rb->size;
    if (size > avail) {
        size = avail;
    }

    size_t rem = rb->capacity - rb->head;
    if (size > rem) {
        memcpy(rb->buffer + rb->head, data, rem);
        memcpy(rb->buffer, data + rem, size - rem);
    } else {
        memcpy(rb->buffer + rb->head, data, size);
    }

    rb->size += size;
    rb->head += size;
    rb->head %= rb->capacity;

    return size;
}

int rb_dequeue(struct ringbuffer *rb, char* data, size_t size)
{
    if (!rb || !rb->buffer) {
        return -EINVAL;
    }

    if (!data) {
        return -EINVAL;
    }

    size_t avail = rb->size;
    if (size > avail) {
        size = avail;
    }

    size_t rem = rb->capacity - rb->tail;
    if (size > rem) {
        memcpy(data, rb->buffer + rb->tail, rem);
        memcpy(data + rem, rb->buffer, size - rem);
    } else {
        memcpy(data, rb->buffer + rb->tail, size);
    }

    rb->size -= size;
    rb->tail += size;
    rb->tail %= rb->capacity;

    return size;
}

int main(void)
{
    struct ringbuffer rb;
    char data[10];

    assert(0 == rb_init(&rb, 10));

    assert(7 == rb_enqueue(&rb, "abcdefg", 7));
    assert(3 == rb_enqueue(&rb, "hijklmn", 7));

    memset(data, 0, sizeof(data));
    assert(7 == rb_dequeue(&rb, data, 7));
    assert(0 == strcmp(data, "abcdefg"));

    assert(7 == rb_enqueue(&rb, "opqrstu", 7));

    memset(data, 0, sizeof(data));
    assert(7 == rb_dequeue(&rb, data, 7));
    assert(0 == strcmp(data, "hijopqr"));

    memset(data, 0, sizeof(data));
    assert(3 == rb_dequeue(&rb, data, 7));
    assert(0 == strcmp(data, "stu"));

    return 0;
}
