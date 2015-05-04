//
// 3.6. Sort a stack in accending order using only 1 additional stack if needed
//

#include "stack.h"
#include <assert.h>
#include <iostream>

template < typename T >
void sort(stack<T>& s)
{
	stack<T> r;
	while (!s.empty()) {
		T val = s.pop();

		// Find a place to insert current value storing skipped elements on s
		while (!r.empty() && (r.top() < val)) {
			s.push(r.pop());
		}

		r.push(val);
	}
	
	s = r;
}

int main(void)
{
	stack<int> s;
	s.push(0);
	s.push(2);
	s.push(5);
	s.push(3);
	s.push(6);
	s.push(7);
	s.push(10);
	s.push(42);

	sort(s);

	assert(0 == s.pop());
	assert(2 == s.pop());
	assert(3 == s.pop());
	assert(5 == s.pop());
	assert(6 == s.pop());
	assert(7 == s.pop());
	assert(10 == s.pop());
	assert(42 == s.pop());

	return 0;
}