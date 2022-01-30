#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <iostream>
#include <stack>

struct list_node {
    struct list_node* next;
    char c;
};

static struct list_node* make_node(struct list_node* prev, char c)
{
    struct list_node* n = (struct list_node*) malloc(sizeof(*n));
    assert(n);
    n->c = c;
    n->next = NULL;
    if (prev)
        prev->next = n;
    return n;
}

bool is_palindrome(const struct list_node* head)
{
    if (!head)
        return false;
        
    std::stack<char> s;
    
    const struct list_node* lead = head;
    const struct list_node* follow = head;
    bool odd = true;
    
    while (true) {
        if (lead->next)
            s.push(follow->c);
            
        lead = lead->next;
        if (!lead)
            break;
        odd = false;
        lead = lead->next;
        if (!lead)
            break;
        odd = true;
        follow = follow->next;
    }

    follow = follow->next;
    while (follow) {
        if (s.top() != follow->c)
            return false;
        s.pop();
        follow = follow->next;
    }
    
    return true;
}

int main()
{
    {
        struct list_node* head = make_node(NULL, 'a');
        make_node(make_node(make_node(make_node(head, 'b'), 'c'), 'b'), 'a');
        assert(is_palindrome(head));
    }
    
    {
        struct list_node* head = make_node(NULL, 'a');
        make_node(make_node(make_node(head, 'b'), 'b'), 'a');
        assert(is_palindrome(head));
    }
    
    {
        struct list_node* head = make_node(NULL, 'a');
        make_node(make_node(head, 'b'), 'b');
        assert(!is_palindrome(head));
    }
    
    {
        struct list_node* head = make_node(NULL, 'a');
        make_node(make_node(make_node(head, 'b'), 'a'), 'b');
        assert(!is_palindrome(head));
    }
    
    {
    
        struct list_node* head = make_node(NULL, 'a');
        assert(is_palindrome(head));
    }
    
    return 0;
}
