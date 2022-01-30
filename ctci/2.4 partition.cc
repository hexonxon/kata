#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <iostream>

struct list_node {
    struct list_node* next;
    int val;
};

struct list_node* partition(struct list_node* head, int x)
{
    struct list_node* follow = head;
    struct list_node* lead = head;
    
    while (follow && follow->val < x) {
        lead = follow = follow->next;
    }
    
    while (lead != NULL) {
        if (lead->val >= x) {
            lead = lead->next;
            continue;
        }
            
        std::swap(follow->val, lead->val);
        follow = follow->next;
        lead = lead->next;
    }

    return head;
}

static struct list_node* make_node(struct list_node* prev, int val)
{
    struct list_node* n = (struct list_node*) malloc(sizeof(*n));
    assert(n);
    n->next = NULL;
    n->val = val;
    if (prev)
        prev->next = n;
    return n;
}

int main()
{
    struct list_node* head = make_node(NULL, 3);
    make_node(make_node(make_node(make_node(make_node(make_node(head, 5), 8), 5), 10), 2), 1);
    
    head = partition(head, 5);
    bool in_left = true;
    while (head) {
        if (!in_left) {
            assert(head->val >= 5);
        } else {
            if (head->val >= 5) {
                in_left = false;
            }
        }
        
        head = head->next;
    }
        
    return 0;
}
