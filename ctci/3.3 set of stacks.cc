#include <iostream>
#include <stack>
#include <vector>
#include <cassert>

class SetOfStacks
{
public:
    
    SetOfStacks(size_t threshold) : m_threshold(threshold)
    {
        assert(threshold != 0);
    }
    
    void push(int val)
    {
        if (m_stacks.empty() || curStack().size() == m_threshold)
            pushStack();
            
        curStack().push(val);
    }
    
    bool pop(int& val)
    {
        if (m_stacks.empty())
            return false;
            
        return popAt(m_stacks.size() - 1, val);
    }
    
    bool popAt(size_t stackIdx, int &val)
    {
        if (stackIdx >= m_stacks.size())
            return false;
            
        auto pstack = m_stacks.begin() + stackIdx;
        assert(!pstack->empty());
        
        val = pstack->top();
        pstack->pop();
        if (pstack->empty())
            m_stacks.erase(pstack);
        
        return true;
    }
    
private:

    std::stack<int>& curStack()
    {
        assert(!m_stacks.empty());
        return m_stacks.back();
    }
    
    void pushStack()
    {
        m_stacks.emplace_back();
    }
    
    void popStack()
    {
        m_stacks.pop_back();
    }
    
    size_t m_threshold;
    std::vector<std::stack<int>> m_stacks;
};

using namespace std;

int main()
{
    SetOfStacks stacks(2);
    stacks.push(1);
    stacks.push(2);
    stacks.push(3);
    
    int val;
    assert(stacks.popAt(0, val) && val == 2);
    assert(stacks.pop(val) && val == 3);
    assert(stacks.pop(val) && val == 1);
    assert(!stacks.pop(val));
    
    return 0;
}
