//
// 3.3. Implement a set of stacks
//

#include <list>
#include <stdexcept>

#include <assert.h>

template < typename T >
class stack 
{
public:

	stack()
	{
	}

	~stack()
	{
	}

	void push(const T& t)
	{
		m_list.push_front(t);
	}

	T pop()
	{
		if (m_list.empty()) {
			throw std::out_of_range("pop: stack is empty");
		}

		const T& t = m_list.front();
		m_list.pop_front();
		return t;
	}

	T& top()
	{
		if (m_list.empty()) {
			throw std::out_of_range("top: stack is empty");
		}

		return m_list.front();		
	}

	const T& top() const
	{
		return const_cast<stack<T>*>(this)->top();
	}

	size_t size() const 
	{
		return m_list.size();
	}

	bool empty() const 
	{
		return size() == 0;
	}

private:

	std::list<T> m_list;	
};

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
		if (m_stackset.empty() || (active_stack().size() == m_limit)) {
			m_stackset.push(stack<T>());
		}

		// Push element into the topmost stack
		assert(active_stack().size() < m_limit);
		active_stack().push(t);
	}

	T pop()
	{
		if (m_stackset.size() == 0) {
			throw std::out_of_range("Stack set is empty");
		}

		T t = active_stack().pop();
		if (active_stack().size() == 0) {
			m_stackset.pop();
		}

		return t;
	}

	const T& top() const
	{
		if (m_stackset.size() == 0) {
			throw std::out_of_range("Stack set is empty");
		}

		return active_stack().top();
	}

	size_t total_stacks() const 
	{
		return m_stackset.size();
	}

private:

	stack<T>& active_stack() {
		return m_stackset.top();
	}

	size_t m_limit;
	stack< stack<T> > m_stackset;
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
