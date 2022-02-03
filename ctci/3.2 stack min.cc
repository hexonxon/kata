#include <vector>
#include <cassert>
#include <stack>

struct minstack {
    std::stack<int> _stack;
    std::stack<int> _min;
    
    void push(int val) {
        _stack.push(val);
        if (val < _min.top() || _min.empty())
            _min.push(val);
    }
    
    bool pop(int& val) {
        if (_stack.empty())
            return false;
        val = _stack.top();
        _stack.pop();
        
        if (val == _min.top())
            _min.pop();
        
        return true;
    }
    
    int min() const {
        return _min.top();
    }
    
    bool empty() const {
        return _stack.empty();
    }
};


int main()
{
    struct minstack ms;
    
    assert(ms.empty());
    ms.push(2); assert(ms.min() == 2);
    ms.push(1); assert(ms.min() == 1);
    ms.push(3); assert(ms.min() == 1);
    ms.push(1); assert(ms.min() == 1);
    ms.push(0); assert(ms.min() == 0);
    
    int val;
    assert(ms.pop(val) && val == 0); assert(ms.min() == 1);
    assert(ms.pop(val) && val == 1); assert(ms.min() == 1);
    assert(ms.pop(val) && val == 3); assert(ms.min() == 1);
    assert(ms.pop(val) && val == 1); assert(ms.min() == 2);
    assert(ms.pop(val) && val == 2); assert(ms.empty());
    assert(!ms.pop(val));
    
    return 0;
}
