#include <stdint.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/mman.h>

/* Stored in slab slot instead of object data when slot is free */
struct slab_free_slot
{
    struct slab_free_slot *next;
};

#define SLAB_SLOT_ALLOCATED 0x534c4142
#define SLAB_SLOT_FREE      0x46524545
#define SLAB_SLOT_FOOTER(s, ptr) \
    ((struct slab_footer*)((void*)(ptr) + (s)->slot_size - sizeof(struct slab_footer)))

/* Attached to every allocation we do */
struct slab_footer
{
    uint32_t marker; /* 'SLAB' of allocated, 'FREE' if not */
};

struct slab
{
    size_t alignment;
    size_t max_objects;
    size_t slot_size;

    void *buffer;
    size_t buffer_size;

    struct slab_free_slot *first_free_slot;
};

#define PAGE_SIZE   4096
#define MAX(a, b)   ((a) > (b) ? (a) : (b))

#define ALIGN_UP(val, align)    ((val) + ((align) - 1) & ~((align) - 1))
#define IS_ALIGNED(val, align)  (((val) & ((align) - 1)) == 0)

int slab_init(struct slab *s, size_t size, size_t alignment, size_t max_objects)
{
    if (!s || !size || !max_objects)
        return -EINVAL;

    if (alignment > PAGE_SIZE)
        return -ENOTSUP;

    if (alignment == 0)
        alignment = 8;

    size_t slot_size = MAX(size, sizeof(struct slab_free_slot)) + sizeof(struct slab_footer);
    size_t aligned_slot_size = ALIGN_UP(slot_size, alignment);
    size_t buffer_size = ALIGN_UP(aligned_slot_size * max_objects, PAGE_SIZE);

    s->buffer = mmap(NULL, buffer_size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    if (s->buffer == MAP_FAILED)
        return -errno;

    s->alignment = alignment;
    s->max_objects = max_objects;
    s->slot_size = slot_size;

    struct slab_free_slot *prev = NULL;
    for (size_t i = 0; i < s->max_objects; ++i) {
        struct slab_free_slot *slot = (struct slab_free_slot *)(s->buffer + i * aligned_slot_size);
        slot->next = NULL;
        if (prev)
            prev->next = slot;
        prev = slot;

        SLAB_SLOT_FOOTER(s, slot)->marker = SLAB_SLOT_FREE;
    }

    s->first_free_slot = s->buffer;
    return 0;
}

void slab_release(struct slab *s)
{
    munmap(s->buffer, s->buffer_size);
}

void *slab_alloc(struct slab *s)
{
    if (!s || !s->first_free_slot)
        return NULL;
    
    /* TODO: relaxed mem ordering */
    struct slab_free_slot *slot;
    do {
        slot = s->first_free_slot;
    } while (!atomic_compare_exchange_strong(&s->first_free_slot, &slot, slot->next));

    memset(slot, 0, s->slot_size);
    SLAB_SLOT_FOOTER(s, slot)->marker = SLAB_SLOT_ALLOCATED;

    return slot;
}

int slab_free(struct slab *s, void *ptr)
{
    if (!ptr)
        return 0;

    if (ptr < s->buffer || ptr >= s->buffer + s->buffer_size || !IS_ALIGNED((uintptr_t)ptr, s->alignment))
        return -EINVAL;

    if (SLAB_SLOT_FOOTER(s, ptr)->marker != SLAB_SLOT_ALLOCATED)
        return -EINVAL;

    SLAB_SLOT_FOOTER(s, ptr)->marker = SLAB_SLOT_FREE;

    /* TODO: memory ordering */
    struct slab_free_slot *slot = (struct slab_free_slot *)ptr;
    do {
        slot->next = s->first_free_slot;
    } while (!atomic_compare_exchange_strong(&s->first_free_slot, &slot->next, slot));

    return 0;
}

#include <assert.h>

int main(void)
{
    struct slab s;
    assert(0 == slab_init(&s, sizeof(unsigned int), 0, 128));

    int *ptrs[128] = {NULL};
    for (int i = 0; i < 128; ++i) {
        ptrs[i] = (int*)slab_alloc(&s);
        assert(ptrs[i] != NULL);
        if (i > 0)
            assert(ptrs[i] != ptrs[i - 1]);
    }

    assert(NULL == slab_alloc(&s));

    for (int i = 0; i < 128; ++i) {
        slab_free(&s, ptrs[i]);
    }

    assert(0 != slab_free(&s, ptrs[0]));

    slab_release(&s);
}
