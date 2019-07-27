#include <vector>
#include <bitset>
#include <cassert>
#include <climits>
#include <memory>
#include <algorithm>

using namespace std;

/// no space constraints version
int find_missing_linear(const vector<int>& a, const vector<int>& b)
{
    unique_ptr<bitset<UINT_MAX>> bmap(new bitset<UINT_MAX>);

    for (auto i : b) {
        bmap->set((unsigned int)i);
    }

    for (auto i : a) {
        if (!bmap->test((unsigned int)i)) {
            return i;
        }
    }

    assert(0);
}

int find_missing_nlogn(vector<int>& a, vector<int>& b)
{
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());

    auto pa = a.begin();
    auto pb = b.begin();

    while (pb != b.end()) {
        if (*pa != *pb) {
            return *pa;
        }

        ++pa;
        ++pb;
    }

    assert(pb == b.end());
    return *pa;
}

int main(void)
{
    vector<int> a = {4, -8, 12, 9, 3};
    vector<int> b = {9, 3, 12, -8};
    assert(find_missing_linear(a, b) == 4);
    assert(find_missing_nlogn(a, b) == 4);
    return 0;
}
