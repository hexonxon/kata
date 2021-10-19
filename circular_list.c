#include <stdlib.h>
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

int main(void)
{
    struct list_node head;
    
    list_head_init(&head);
    assert(head.next == &head && head.prev == &head);

    struct list_node node;
    list_insert_after(&head, &node);
    assert(head.next == &node && node.next == &head);
    assert(head.prev == &node && node.prev == &head);

    list_delete(&node);
    assert(head.next == &head && head.prev == &head);

    list_delete(&head);
    assert(head.next == NULL && head.prev == NULL);
}
