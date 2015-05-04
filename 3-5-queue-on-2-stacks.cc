//
// 3.5. Implement a queue on 2 stacks
//

#include "stack.h"
#include <assert.h>

template < typename T >
class queue
{
public:

	void add(const T& t)
	{
		// Just add to newest
 		m_newest.push(t);
	}

	T remove()
	{
		repopulate_oldest();
		return m_oldest.pop();
	}

	const T& peek()
	{
		repopulate_oldest();
		return m_oldest.top();
	}

	size_t size() const 
	{
		return m_newest.size() + m_oldest.size();
	}

	bool is_empty() const
	{
		return size() == 0;
	}

protected:

	void repopulate_oldest()
	{
		// re-populate oldest stack if needed
		if (m_oldest.empty()) {
			while (!m_newest.empty()) {
				int val = m_newest.pop();
				m_oldest.push(val);
			}
		}
	}

private:

	stack<T> m_newest;
	stack<T> m_oldest;
};

int main(void)
{
	queue<int> q;
	q.add(0);
	q.add(1);
	q.add(2);

	assert(q.size() == 3);

	assert(0 == q.peek());
	assert(0 == q.remove());
	assert(1 == q.peek());
	assert(1 == q.remove());
	assert(2 == q.peek());
	assert(2 == q.remove());

	assert(q.size() == 0);	

	return 0;
}
