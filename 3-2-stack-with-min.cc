//
// 3.2. Design a stack data structure that provides additional "min" operation which returns the minimum element in O(1)
// ---------------------------------------------------------------------------------------------------------------------
//
// We will use an additional stack that will contains pointers to least values in main stack as they appear.
// When pushing a new value we will check if it is the new minimum and push a pointer to it on the minstack.
// When popping a value it will always either be on the top of the minstack or not in the minstack at all due to the LIFO nature of the stack.
// Thus we can safely check if popped value is on the minstack top and remove it from there if it is this way exposing the next least value.
//

#include <list>
#include <exception>
#include <stdexcept>

#include <assert.h>

using namespace std;

template < typename T >
class minstack
{
public:

	minstack()
	{
	}

	~minstack()
	{
	}

	const T& top() {
		if (m_stack.empty()) {
			throw std::out_of_range("Stack is empty");
		}

		return m_stack.front();
	}

	void push(const T& t)
	{
		// Add new element
		m_stack.push_front(t);

		// See if new element is the new minimum value and add a pointer to it to the minstack
		if (m_minstack.empty() || (min() > top())) {
			m_minstack.push_front(&m_stack.front());
		} 
	}

	T pop()
	{
		if (m_stack.empty()) {
			throw std::out_of_range("Stack is empty");
		}

		assert(!m_minstack.empty());


		// See if removed element is the minimum one and pop a pointer to it from the min stack exposing the next least element
		if (m_minstack.front() == &m_stack.front()) {
			m_minstack.pop_front();
		}

		const T& t = m_stack.front();
		m_stack.pop_front();
		return t;
	}

	const T& min()
	{
		return *m_minstack.front();
	}

private:

	// Copying will mess up minstack pointers, so either implement it properly or forbid for now
	minstack(const minstack<T>& other);
	minstack<T>& operator = (const minstack<T>& other);
	
	std::list<T> m_stack;
	std::list<T*> m_minstack;
};


int main(void)
{
	minstack<int> stack;

	stack.push(3);
	stack.push(1);
	stack.push(0);
	stack.push(2);
	stack.push(0);

	assert(0 == stack.min());
	
	assert(0 == stack.pop());
	assert(0 == stack.min());

	assert(2 == stack.pop());
	assert(0 == stack.min());

	assert(0 == stack.pop());
	assert(1 == stack.min());

	assert(1 == stack.pop());
	assert(3 == stack.min());

	assert(3 == stack.pop());

	return 0;
}
