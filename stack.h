#ifndef KATA_STACK_H
#define KATA_STACK_H

#include <cstddef>
#include <list>
#include <stdexcept>

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

#endif
