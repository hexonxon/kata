#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <assert.h>

struct list_node
{
    struct list_node *next;
    struct list_node *prev;
};

void list_head_init(struct list_node* n)
{
    n->prev = n;
    n->next = n;
}

void list_insert_after(struct list_node *n, struct list_node *new)
{
    new->prev = n;
    new->next = n->next;
    n->next->prev = new;
    n->next = new;
}

void list_delete(struct list_node *n)
{
    struct list_node *next = n->next;
    struct list_node *prev = n->prev;

    prev->next = next;
    next->prev = prev;
    n->next = n->prev = NULL;
}

struct lru_cache_entry
{
    uint64_t key;
    void *data;

    struct list_node link;
};

struct lru_cache
{
    struct list_node entries;

    /* This is just to emulate a hash table for the sake of the excercise */
    struct lru_cache_entry *lookup[128];

    size_t capacity;
    size_t size;
};

int lru_cache_init(struct lru_cache *cache, size_t capacity)
{
    if (!cache)
        return -EINVAL;

    cache->capacity = capacity;
    cache->size = 0;
    list_head_init(&cache->entries);
    memset(cache->lookup, 0, sizeof(cache->lookup));

    return 0;
}

void* lru_cache_fetch(struct lru_cache *cache, uint64_t addr)
{
    struct lru_cache_entry *entry = cache->lookup[addr];
    if (!entry) {
        /* Populate an entry simulating fetch from lower-level storage */
        entry = (struct lru_cache_entry*) malloc(sizeof(*entry));
        if (!entry)
            return NULL;

        entry->key = addr;
        entry->data = entry; /* Faking unique data here */

        struct list_node *tail = cache->entries.prev;
        list_insert_after(tail, &entry->link);
        if (cache->size < cache->capacity) {
            ++cache->size;
        } else {
            list_delete(tail);
        }
    } else {
        list_delete(&entry->link);
        list_insert_after(&cache->entries, &entry->link);
    }

    return entry->data;
}

int main(void)
{
}
