#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <iostream>

struct list_node {
    struct list_node* next;
};

const struct list_node* kth_to_last(const struct list_node* head, size_t k)
{
    if (!head)
        return NULL;
        
    const struct list_node* node = head;
    while (node && k > 0) {
        node = node->next;
        --k;
    }
        
    if (k != 0)
        return NULL; /* Overshoot */
        
    while (node->next) {
        node = node->next;
        head = head->next;
    }
    
    return head;
}

static struct list_node* make_node(struct list_node* prev)
{
    struct list_node* n = (struct list_node*) malloc(sizeof(*n));
    assert(n);
    n->next = NULL;
    if (prev)
        prev->next = n;
    return n;
}

int main()
{
    struct list_node* head = make_node(NULL);
    make_node(make_node(head));
        
    assert(head->next->next == kth_to_last(head, 0));
    assert(head->next == kth_to_last(head, 1));
    assert(head == kth_to_last(head, 2));
    assert(NULL == kth_to_last(head, 3));
    assert(NULL == kth_to_last(NULL, 0));
    
    return 0;
}
