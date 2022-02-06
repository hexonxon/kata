#include <iostream>
#include <stack>
#include <cassert>

class QueueViaStacks
{
public:
    
    void push(int val)
    {
        m_pushstack.push(val);
    }
    
    bool pop(int& val)
    {
        if (m_popstack.empty()) {
            if (m_pushstack.empty())
                return false;
                
            while (!m_pushstack.empty()) {
                m_popstack.push(m_pushstack.top());
                m_pushstack.pop();
            }
        }
        
        assert(!m_popstack.empty());
        val = m_popstack.top();
        m_popstack.pop();
        return true;
    }

private:

    std::stack<int> m_pushstack;
    std::stack<int> m_popstack;
};

using namespace std;

int main()
{
    int val;
    QueueViaStacks q;
    q.push(1);
    q.push(2);
    q.push(3);
    assert(q.pop(val) && val == 1);
    q.push(4);
    assert(q.pop(val) && val == 2);
    assert(q.pop(val) && val == 3);
    assert(q.pop(val) && val == 4);
    assert(!q.pop(val));
    
    
    return 0;
}
