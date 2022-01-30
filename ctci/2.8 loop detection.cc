#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <iostream>
#include <unordered_set>

struct list_node {
    struct list_node* next;
};

const struct list_node* find_loop_start(const struct list_node* head)
{
    if (!head)
        return NULL;
 
    const struct list_node* lead = head;
    const struct list_node* follow = head;
    
    while (lead) {
        lead = lead->next;
        if (!lead)
            break;
        
        lead = lead->next;    
        follow = follow->next;
        
        if (lead == follow)
            return lead;
    }
    
    return NULL;
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
    {
        struct list_node* a = make_node(NULL);
        struct list_node* b = make_node(a);
        struct list_node* c = make_node(b);
        assert(NULL == find_loop_start(a));
    }
    
    {
        struct list_node* a = make_node(NULL);
        struct list_node* b = make_node(a);
        struct list_node* c = make_node(b);
        struct list_node* d = make_node(c);
        d->next = b;

        assert(d == find_loop_start(a));
    }

    return 0;
}
