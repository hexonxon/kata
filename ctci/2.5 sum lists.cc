#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <iostream>

struct list_node {
    struct list_node* next;
    unsigned digit;
};

static struct list_node* make_node(struct list_node* prev, unsigned digit)
{
    struct list_node* n = (struct list_node*) malloc(sizeof(*n));
    assert(n);
    n->digit = digit;
    n->next = NULL;
    if (prev)
        prev->next = n;
    return n;
}

struct list_node* sum_lists(const struct list_node* a, const struct list_node* b)
{
    if (!a || !b)
        return NULL;
    
    struct list_node* sum_head = NULL;
    struct list_node* sum_tail = NULL;
    unsigned carry = 0;
    
    while (a || b) {
        unsigned sum = (a ? a->digit : 0) + (b ? b->digit : 0) + carry;
        carry = sum / 10;
        sum_tail = make_node(sum_tail, sum % 10);
        if (!sum_head)
            sum_head = sum_tail;
            
        a = a->next;
        b = b->next;
    }
    
    if (carry) {
        make_node(sum_tail, carry);
    }
    
    return sum_head;
}

int main()
{
    struct list_node* a = make_node(NULL, 7);
    make_node(make_node(a, 1), 6);
    
    struct list_node* b = make_node(NULL, 3);
    make_node(make_node(b, 8), 3);
    
    struct list_node* s = sum_lists(a, b);
    assert(s);
    assert(s->digit == 0);
    assert(s->next->digit == 0);
    assert(s->next->next->digit == 0);
    assert(s->next->next->next->digit == 1);
    assert(s->next->next->next->next == NULL);

    return 0;
}
