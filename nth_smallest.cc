#include <vector>
#include <queue>
#include <cassert>

using namespace std;

// if m is the size of the array,
// straightforward solution would be sorting, which is mlogm
// using maxheap is m*log(n)
int nth_smallest(const vector<int>& v, size_t n)
{
    assert(n > 0);
    assert(n <= v.size());

    priority_queue<int> maxheap;

    auto next = v.begin();
    for (size_t i = 0; i < n; ++i) {
        maxheap.push(*next++);
    }

    while (next != v.end()) {
        maxheap.push(*next++);
        maxheap.pop();
    }

    return maxheap.top();
}

int main(void)
{
    vector<int> v = {4, 8, 12, 5, -3};
    assert(nth_smallest(v, 1) == -3);
    assert(nth_smallest(v, 2) == 4);
    assert(nth_smallest(v, 3) == 5);
    assert(nth_smallest(v, 4) == 8);
    assert(nth_smallest(v, 5) == 12);

    return 0;
}
