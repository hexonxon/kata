//
// 2.5 Add two numbers represented by linked lists and return the sum as a list
// ----------------------------------------------------------------------------
//

#include <stdlib.h>
#include <assert.h>

#include <iostream>
#include <list>

static long to_number_recoursive(std::list<int>::const_iterator i, std::list<int>::const_iterator end)
{
	if (i == end) {
		return 0;
	}

	int digit = *i;
	return to_number_recoursive(++i, end) * 10 + digit;
}

static long to_number(const std::list<int>& a)
{
	return to_number_recoursive(a.begin(), a.end());
}

static std::list<int> add(const std::list<int>& a, const std::list<int>& b)
{
	std::list<int> res;

	auto i = a.begin();
	auto j = b.begin();

	int carry = 0;
	while (i != a.end() && j != b.end()) {
		int sum = *i + *j + carry;
		carry = sum / 10;
		res.push_back(sum % 10);

		++i;
		++j;
	}

	if (i != a.end()) {
		res.push_back(*i + carry);
		carry = 0;
		++i;
	} else if (j != b.end()) {
		res.push_back(*j + carry);
		carry = 0;
		++j;
	}

	return res;
}

int main(void)
{
	std::list<int> a;
	a.push_back(7);
	a.push_back(1);
	a.push_back(6);

	std::list<int> b;
	b.push_back(8);
	b.push_back(4);
	b.push_back(0);
	b.push_back(2);

	std::cout << to_number(add(a, b)) << std::endl;
	assert((2048 + 617) == to_number(add(a, b)));
	return 0;
}