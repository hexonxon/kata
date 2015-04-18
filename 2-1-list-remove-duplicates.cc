//
// 2.1  Remove duplicates from an unsorted linked list
// 
// Questions:
// 1. Stored value ranges. Affects possible bitmap solution
// ----------------------------------------------------------------------------------
//
// 1. We can sort the list and then remove all duplicates in a single pass which would yeild O(nlogn + n). Depends on data representation.
// 2. We can have a pointer chasing algorithm which ammortizes to O(n^2). Can be used for it's O(1) memory and when sorting is unavailable.
// 3. We can use a hash table covering the value ranges. 
//  


#include <stdlib.h>
#include <assert.h>

#include <iostream>
#include <list>
#include <unordered_map>

using namespace std;

// 1. We can sort the list and then remove all duplicates in a single pass which would yeild O(nlogn + n). Depends on data representation.

template < typename T >
static void remove_duplicates_sort(std::list<T>& list)
{
	list.sort([&] (const int& a, const int& b) -> bool { return a < b; });

	auto i = list.begin();
	while (i != list.end()) 
	{
		auto j = i; ++j;
		while (*j == *i) 
		{
			auto next = j; ++next;
			list.erase(j);
			j = next;
		}

		++i;
	}
}

// 2. We can have a pointer chasing algorithm which ammortizes to O(n^2). Can be used for it's O(1) memory and when sorting is unavailable.

template < typename T >
static void remove_duplicates_constant_space(std::list<T>& list)
{
	auto i = list.begin();
	while (i != list.end()) 
	{
		auto j = i; ++j;
		while (j != list.end()) 
		{
			if (*i == *j) {
				auto next = j; ++next;
				list.erase(j);
				j = next;
			} else {
				++j;
			}
		}

		++i;
	}
}

// 3. We can use a hash table covering the value ranges. 

template < typename T >
static void remove_duplicates_constant_time(std::list<T>& list)
{
	unordered_map<T, bool> hashmap;

	auto i = list.begin();
	while (i != list.end()) {
		if (hashmap.end() != hashmap.find(*i)) {
			auto next = i;
			++next;

			list.erase(i);
			i = next;
		} else {
			hashmap[*i++] = true;
		}
	}
}

typedef void (*func) (std::list<int>&);

static void test(func f)
{
	std::list<int> testlist;
	testlist.push_back(1);
	testlist.push_back(2);
	testlist.push_back(3);
	testlist.push_back(1);
	testlist.push_back(4);
	testlist.push_back(2);

	f(testlist);

	auto i = testlist.begin();
	while (i != testlist.end()) 
	{
		auto j = i;
		++j;
		while (j != testlist.end()) 
		{
			assert(*j != *i);
			++j;
		}

		++i;
	}
}

int main(void)
{
	test(remove_duplicates_sort);
	test(remove_duplicates_constant_time);
	test(remove_duplicates_constant_space);
	return 0;
}