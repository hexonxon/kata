#include <iostream>
#include <cassert>

struct list_head {
    struct list_head* next;
    char c;
};

static struct list_head* make_head(struct list_head* prev, char c)
{
    struct list_head* h = (struct list_head*) malloc(sizeof(*h));
    assert(h);
    
    h->next = NULL;
    h->c = c;
    if (prev)
        prev->next = h;
        
    return h;
}

const struct list_head* find_loop_start(const struct list_head* head)
{
    const struct list_head* slow = head;
    const struct list_head* fast = head;

    do {
        if (!fast || !fast->next)
            return NULL;
            
        fast = fast->next->next;
        slow = slow->next;
    } while (fast != slow);
    
    /* Meeting point is K steps before loop start, where K is the length of unlooped portion */
    slow = head;
    while (fast != slow) {
        fast = fast->next;
        slow = slow->next;
    }
    
    return slow;
}

int main()
{
    struct list_head* a = make_head(NULL, 'a');
    struct list_head* b = make_head(a, 'b');
    struct list_head* c = make_head(b, 'c');
    struct list_head* d = make_head(c, 'd');
    d->next = b;
    
    assert(find_loop_start(a) == b);
    return 0;
}
