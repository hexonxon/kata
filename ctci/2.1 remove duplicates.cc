#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <unordered_set>
#include <iostream>

struct list_node {
    struct list_node* next;
    int val;
};

struct list_node* remove_duplicates(struct list_node* head)
{
    struct list_node* node = head;
    struct list_node* prev = NULL;
    std::unordered_set<int> lookup;
    
    while (node != NULL) {
        struct list_node* next = node->next;
        if (lookup.find(node->val) != lookup.end()) {
            assert(prev); /* If this is the first node, no duplicates are possible yet */
            
            prev->next = next;
            free(node); /* TODO: probably something more complicated then 'free' */
        } else {
            lookup.insert(node->val);
            prev = node;
        }
        
        node = next;
    }
    
    return head;
}

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

int main()
{
    {
        struct list_node* head = make_node(NULL, 1);
        make_node(make_node(make_node(head, 3), 1), 2);
        
        struct list_node* new_head = remove_duplicates(head);
        assert(new_head->val == 1);
        assert(new_head->next->val == 3);
        assert(new_head->next->next->val == 2);
        assert(new_head->next->next->next == NULL);
        
        /* Now there are no duplicates */
        new_head = remove_duplicates(new_head);
        assert(new_head->val == 1);
        assert(new_head->next->val == 3);
        assert(new_head->next->next->val == 2);
        assert(new_head->next->next->next == NULL);
    }
    
    {
        assert(NULL == remove_duplicates(NULL));
    }
    
    return 0;
}
