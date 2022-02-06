#include <iostream>
#include <stack>
#include <cassert>

static size_t move_elements(std::stack<int>& from, std::stack<int>& to, size_t count)
{
    size_t nmoved = 0;
    while (!from.empty() && count-- > 0) {
        to.push(from.top());
        from.pop();
    }
    
    return nmoved;
}

void sort_stack(std::stack<int>& s)
{
    if (s.size() < 2)
        return;

    std::stack<int> tmp;
    while (!s.empty()) {
        int v = s.top();
        s.pop();
        
        /* Keep tmp in sorted descending order */
        while (!tmp.empty() || v < tmp.top()) {
            move_elements(tmp, s, 1);
        }
        tmp.push(v);
    }

    move_elements(tmp, s, tmp.size());
}

int main()
{
    std::stack<int> s;
    s.push(4);
    s.push(1);
    s.push(5);
    s.push(3);
    
    sort_stack(s);
    assert(s.top() == 1); s.pop();
    assert(s.top() == 3); s.pop();
    assert(s.top() == 4); s.pop();
    assert(s.top() == 5); s.pop();
    
    return 0;
}
