#include <vector>
#include <set>
#include <algorithm>
#include <iostream>
#include <cassert>

using namespace std;

// Mutable version, works in nlogn, with O(1) memory
size_t num_pairs(vector<int>& v, int k)
{
    if (v.size() < 2) {
        return 0;
    }

    sort(v.begin(), v.end());

    if (k <= v.front()) {
        return 0;
    }

    // Find end of valid sum range.
    // This can be made better by binary search
    int largest = k - v.front();
    auto low = v.begin();
    auto high = low;
    while (high != v.end() && *high <= largest) {
        ++high;
    }

    --high;

    if (high == v.end() || high == low) {
        return 0;
    }

    size_t res = 0;
    while (low < high) {
        int sum = *low + *high;

        if (sum == k) {
            cout << "(" << *low << ", " << *high << ")" << endl;
            ++res;

            // Skip same values on both sides
            int a = *low;
            int b = *high;
            while (a == *low && low < high) {
               ++low;
            }

            while (b == *high && low < high) {
               --high;
            }
        } else if (sum < k) {
            ++low;
        } else if (sum > k) {
            --high;
        }
    }

    return res;

}

// Immutable version, works in nlogn as well, takes O(n) memory for additional set when we can't change input array
size_t num_pairs_immutable(const vector<int>& v, int k)
{
    if (v.size() < 2) {
        return 0;
    }

    set<int> vals;
    for (auto i : v) {
        vals.insert(i);
    }

    size_t res = 0;
    for (auto i : vals) {
        auto pval = vals.find(k - i);
        if (pval != vals.end()) {
            cout << "(" << i << ", " << *pval << ")" << endl;
            vals.erase(pval); // remove to avoid finding a reverse pair later on
            ++res;
        }
    }

    return res;
}

template <typename vector_type>
static void test(size_t(*pfunc)(vector_type& v, int k))
{
    {
        // (-1, 1, 2, 2, 3, 3, 4, 5, 6), 7, 46
        vector<int> v = {1, 3, 2, 3, 2, 5, 46, 6, 7, 4, -1};
        assert(pfunc(v, 5) == 3);
    }

    {
        vector<int> v = {};
        assert(pfunc(v, 5) == 0);
    }

    {
        vector<int> v = {1};
        assert(pfunc(v, 1) == 0);
    }

    {
        vector<int> v = {2, 4, 6};
        assert(pfunc(v, 1) == 0);
    }

    {
        vector<int> v = {2, 4, 6};
        assert(pfunc(v, 99) == 0);
    }
}

int main(void)
{
    test(num_pairs);
    test(num_pairs_immutable);

    return 0;
}
