//
// 2.3  Partition list around a value X
// ------------------------------------
//
// Solution is to scan a list skipping all values less than X. First element >= X is a pivot.
// After that all further values < X are swapped with a pivot and pivot is incremented to point to the next element.
// ----
// Actually that is a perfectly good solution for an array. For the list we can just use 2 lists.
//

#include <stdlib.h>
#include <assert.h>

#include <iostream>
#include <list>

using namespace std;

static void partition(std::list<int>& list, int x)
{
	auto i = list.begin();
	
	// 1. Find a pivot point - position where we encounter the first element >= x
	auto pivot = list.end();
	while (i != list.end()) 
	{
		if (*i >= x) {
			pivot = i;
			break;
		}

		++i;
	}

	if (i == list.end()) {
		// List was entirily less than a value
		return;
	}

	// 2. Starting from a pivot scan the list forward and insert all values less than x before the pivot
	while (i != list.end()) 
	{
		++i;

		if (*i < x) {
			// Swap pivot and current values
			int tmp = *i;
			*i = *pivot;
			*pivot = tmp;

			// Advance pivot to next element (which is >= x as well)
			++pivot;
		}

	}
}

static bool is_partitioned(const list<int>& l, int x)
{
	cout << endl << "partition by " << x << endl;

	auto i = l.begin();
	while (i != l.end())
	{
		if (*i >= x) {
			cout << "--" << endl; 
			break;
		} else {
			cout << *i << endl;
		}

		++i;
	}

	if (i == l.end()) {
		cout << "--" << endl; 
		return true;
	}

	while (i != l.end()) 
	{
		cout << *i << endl;

		if (*i < x) {
			return false;
		}

		++i;
	}

	return true;
}

static void dump(const list<int>& l)
{
	auto i = l.begin();
	while (i != l.end()) 
	{
		cout << *i++ << endl;
	}

	cout << endl;
}

int main (void) 
{
	list<int> l;
	l.push_back(7);
	l.push_back(42);
	l.push_back(31);
	l.push_back(1);
	l.push_back(6);
	l.push_back(23);
	l.push_back(2);
	
	partition(l, 999);
	assert(is_partitioned(l, 999));

	partition(l, 20);
	assert(is_partitioned(l, 20));

	partition(l, 0); 
	assert(is_partitioned(l, 0));


	return 0;
}
