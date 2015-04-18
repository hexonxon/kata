//
// 2.2 Implement an algorithm to find the kth to last element of a singly linked list
// ----------------------------------------------------------------------------------
// 
// Questions:
// 1. What are the complexity requirements
// 2. Do we have a length of linked list pre-calculated? (assume NO)
// 
// Solution:

//    

// 

#include <stdlib.h>
#include <assert.h>

#include <iostream>
#include <vector>
#include <list>

using namespace std;

//
// 1. We can create a parallel random-access data structure to keep track of node pointers. 
//    This will double the memory requirements but runtime will be O(N) ammortized (i.e we may have to reallocate random-access container)
//

template < typename T >
static typename std::list<T>::iterator kth_to_last_indexed(std::list<T>& list, size_t k)
{
	if (k == 0) {
		return list.end();
	}

	std::vector<typename std::list<T>::iterator> v;

	// build list indexes
	typename std::list<T>::iterator i = list.begin();
	while (i != list.end()) {
		v.push_back(i);
		++i;
	}

	// get kth-to-last 
	if (k > v.size()) {
		return list.end();
	}

	return v[v.size() - k];
}

//
// 2. We can recourse into list with a depth counter. Same O(N).
//

template < typename T >
static typename std::list<T>::iterator kth_to_last_recoursive_iter(typename std::list<T>::iterator iter, typename std::list<T>::iterator end, size_t k, size_t& i)
{
	if (iter == end) {
		return end;
	}

	typename std::list<T>::iterator next = iter;
	typename std::list<T>::iterator res = kth_to_last_recoursive_iter<T>(++next, end, k, i);
	++i;

	if (i == k) {

		return iter;
	}

	return res;
}

template < typename T >
static typename std::list<T>::iterator kth_to_last_recoursive(std::list<T>& list, size_t k)
{
	if (k == 0) {
		return list.end();
	}

	size_t i = 0;
	return kth_to_last_recoursive_iter<T>(list.begin(), list.end(), k, i);
}

//
// 3. We can iterate through the list with 2 pointers - one of them ahead of the other by k elements. 
//    When ahead pointer reaches the end the first one will be exactly k nodes behind
//

template < typename T >
static typename std::list<T>::iterator kth_to_last_iterative(std::list<T>& list, size_t k)
{
	if (k == 0) {
		return list.end();
	}

	typename std::list<T>::iterator p1 = list.begin();
	typename std::list<T>::iterator p2 = list.begin();

	// Put p1 k elements ahead
	while ((p1 != list.end()) && (k-- > 0)) {
		++p1;
	}

	if ((p1 == list.end()) && (k != 0)) {
		return list.end();
	}

	// Increment both pointers now. When p1 reaches end, p2 will be k elements behind it
	while (p1 != list.end()) {
		++p1;
		++p2;
	}

	return p2;
}

typedef std::list<int>::iterator (*func) (std::list<int>&, size_t);

static void test(func f)
{
	std::list<int> testlist;

	testlist.push_back(1);
	testlist.push_back(2);
	testlist.push_back(3);
	testlist.push_back(4);
	testlist.push_back(5);
	testlist.push_back(6);

	assert(6 == *(f(testlist, 1)));
	assert(5 == *(f(testlist, 2)));
	assert(4 == *(f(testlist, 3)));
	assert(3 == *(f(testlist, 4)));
	assert(2 == *(f(testlist, 5)));
	assert(1 == *(f(testlist, 6)));

	assert(testlist.end() == f(testlist, 0));
	assert(testlist.end() == f(testlist, 7));
}

int main(void)
{
	cout << "kth_to_last_indexed\n";
	test(kth_to_last_indexed<int>);

	cout << "kth_to_last_recoursive\n";
	test(kth_to_last_recoursive<int>);

	cout << "kth_to_last_iterative\n";
	test(kth_to_last_iterative<int>);

	return 0;
}
