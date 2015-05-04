//
// 3.3. Implement a set of stacks
//

#include "stack.h"

#include <vector>
#include <stdexcept>

#include <assert.h>

template < typename T >
class multistack
{
public:

	multistack(size_t limit) : m_limit(limit)
	{
		if (limit == 0) {
			throw std::invalid_argument("Limit should not be 0");
		}
	}

	~multistack()
	{
	}

	void push(const T& t)
	{
		// Check current stack for limit and create a new stack if needed
		if (m_stacks.empty() || (active_stack().size() == m_limit)) {
			m_stacks.push_back(stack<T>());
		}

		// Push element into the topmost stack
		assert(active_stack().size() < m_limit);
		active_stack().push(t);
	}

	T pop()
	{
		if (m_stacks.empty()) {
			throw std::out_of_range("Stack set is empty");
		}

		T t = active_stack().pop();
		if (active_stack().size() == 0) {
			m_stacks.pop_back();
		}

		return t;
	}

	const T& top() const
	{
		if (m_stacks.empty() == 0) {
			throw std::out_of_range("Stack set is empty");
		}

		return active_stack().top();
	}

	size_t total_stacks() const 
	{
		return m_stacks.size();
	}

private:

	stack<T>& active_stack() {
		return m_stacks.back();
	}

	size_t m_limit;
	std::vector< stack<T> > m_stacks;
};

int main()
{
	multistack<int> mstack(2);

	mstack.push(0);
	assert(1 == mstack.total_stacks());

	mstack.push(1);
	assert(1 == mstack.total_stacks());

	mstack.push(2);
	assert(2 == mstack.total_stacks());

	mstack.push(3);
	assert(2 == mstack.total_stacks());

	mstack.push(4);
	assert(3 == mstack.total_stacks());

	assert(4 == mstack.pop());
	assert(2 == mstack.total_stacks());

	assert(3 == mstack.pop());
	assert(2 == mstack.total_stacks());
	
	assert(2 == mstack.pop());
	assert(1 == mstack.total_stacks());

	assert(1 == mstack.pop());
	assert(1 == mstack.total_stacks());

	assert(0 == mstack.pop());
	assert(0 == mstack.total_stacks());
	
	return 0;
}
