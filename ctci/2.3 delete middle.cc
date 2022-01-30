#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <iostream>

struct list_node {
    struct list_node* next;
    int val;
};

static struct list_node* make_node(struct list_node* prev, int val)
{
    struct list_node* n = (struct list_node*) malloc(sizeof(*n));
    assert(n);
    n->val = val;
    n->next = NULL;
    if (prev)
        prev->next = n;
    return n;
}

void delete_middle(struct list_node* n)
{
    if (!n || !n->next)
        return;
     
    struct list_node* next = n->next;
    n->val = next->val;
    n->next = next->next;
    free(next);
}

int main()
{
    struct list_node* a = make_node(NULL, 1);
    struct list_node* b = make_node(a, 2);
    struct list_node* c = make_node(b, 3);
    
    delete_middle(b);
    assert(a->val == 1);
    assert(a->next->val == 3);
    assert(a->next->next == NULL);

    return 0;
}
