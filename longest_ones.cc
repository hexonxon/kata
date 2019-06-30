/*
 * Find length of the longest sequence of 1-s in a string: 0011011110 -> 4
 */

#include <vector>
#include <cassert>

using namespace std;

size_t longest_ones(const vector<int>& v)
{
    if (v.empty()) {
        return 0;
    }

    size_t max = 0;
    auto first = v.begin();
    auto next = first;

    while (next != v.end()) {
        if (*next == 0) {
            size_t len = next - first;
            if (len > max) {
                max = len;
            }

            first = ++next;
        } else if (*next == 1) {
            ++next;
        }
    }

    if (first != v.end()) {
        size_t len = next - first;
        if (len > max) {
            max = len;
        }
    }

    return max;
}

int main(void)
{
    {
        vector<int> v = {0, 0, 1, 1, 0, 1, 1, 1, 1, 1};
        assert(5 == longest_ones(v));
    }

    {
        vector<int> v = {0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0};
        assert(5 == longest_ones(v));
    }

    {
        vector<int> v = {0, 0, 0, 0};
        assert(0 == longest_ones(v));
    }

    {
        vector<int> v = {0};
        assert(0 == longest_ones(v));
    }

    {
        vector<int> v = {1};
        assert(1 == longest_ones(v));
    }
}
