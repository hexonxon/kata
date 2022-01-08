/**
 * You are given a linked list where the node has two pointers.
 * The first is the regular ‘next’ pointer.
 * The second pointer is called ‘arbitrary_pointer’ and it can point to any node in the linked list.
 *
 * Your job is to write code to make a deep copy of the given linked list.
 * Here, deep copy means that any operations on the original list (inserting, modifying and removing)
 * should not affect the copied list.
 */

#include <cstdlib>
#include <cassert>
#include <unordered_map>

struct list_head {
    struct list_head *next;
    struct list_head *arbitrary;
};

struct list_head *deep_copy(const struct list_head *head)
{
    struct list_head *new_head = NULL;
    struct list_head *prev = NULL;

    std::unordered_map<const struct list_head*, struct list_head*> old_to_new;

    /* Pass 1: do a deep copy setting next pointers and keeping arbitrary ones */
    const struct list_head *node = head;
    while (node) {
        struct list_head *copy = (struct list_head*) malloc(sizeof(*copy));
        copy->next = NULL;
        copy->arbitrary = node->arbitrary;

        if (!prev) {
            new_head = copy;
        } else {
            prev->next = copy;
        }

        old_to_new[node] = copy;

        prev = copy;
        node = node->next;
    }

    /* Pass 2: fixup arbitrary pointers */
    struct list_head *new_node = new_head;
    while (new_node) {
        new_node->arbitrary = old_to_new[new_node->arbitrary];
        new_node = new_node->next;
    }

    return new_head;
}

static struct list_head *prepend(struct list_head *head)
{
    struct list_head *node = (struct list_head *) malloc(sizeof(*node));
    assert(node);
    node->next = head;
    return node;
}

int main(void)
{
    {
        /* All arbitrary pointers are set to the node itself */
        struct list_head *head = prepend(prepend(prepend(NULL)));
        struct list_head *node = head;
        while (node != NULL) {
            node->arbitrary = node;
            node = node->next;
        }

        struct list_head *copy = deep_copy(head);
        while (head != NULL) {
            assert(head != copy);
            assert(copy->arbitrary != head->arbitrary);
            assert(copy->arbitrary == copy);

            head = head->next;
            copy = copy->next;
        }
    }

    {
        /* Empty list */
        assert(deep_copy(NULL) == NULL);
    }
}

