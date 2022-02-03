#include <vector>
#include <cassert>

static std::vector<int> g_array;

struct stack {
    size_t base;
    size_t capacity;
    size_t top;
};

void init(struct stack* s, size_t base, size_t capacity)
{
    s->base = s->top = base;
    s->capacity = capacity;
}

bool push(struct stack* s, int val)
{
    assert(s);
    
    if (s->top - s->base == s->capacity)
        return false;
    
    g_array[s->top++] = val;
    return true;
}

bool pop(struct stack* s, int* val)
{
    assert(s);
    assert(val);
    
    if (s->top == s->base)
        return false;
    
    *val = g_array[--s->top];
    return true;
}

int main()
{
    struct stack s1;
    struct stack s2;
    struct stack s3;
    
    g_array = std::vector<int>(6);
    init(&s1, 0, 2);
    init(&s2, 2, 2);
    init(&s3, 4, 2);
    
    assert(push(&s1, 0));
    assert(push(&s1, 1));
    assert(!push(&s1, 2));
    
    assert(push(&s2, 2));
    assert(push(&s2, 3));
    assert(!push(&s2, 4));
    
    assert(push(&s3, 4));
    assert(push(&s3, 5));
    assert(!push(&s3, 6));
    
    int val;
    assert(pop(&s1, &val) && val == 1);
    assert(pop(&s1, &val) && val == 0);
    assert(!pop(&s1, &val));
    
    assert(pop(&s2, &val) && val == 3);
    assert(pop(&s2, &val) && val == 2);
    assert(!pop(&s2, &val));
    
    assert(pop(&s3, &val) && val == 5);
    assert(pop(&s3, &val) && val == 4);
    assert(!pop(&s3, &val));
    
    return 0;
}
