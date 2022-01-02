/**
 * Given an array of integers and a value, determine if there are any two integers
 * in the array whose sum is equal to the given value.
 */

#include <unordered_set>
#include <vector>
#include <cassert>

/* Make a hash table, put integers and lookup val - i at the same time */
bool has_sum_of_2(const std::vector<int>& data, int val)
{
    std::unordered_set<int> hash;

    for (int i : data) {
        if (hash.find(val - i) != hash.end()) {
            return true;
        }

        /* Insert after lookup so that we don't stumble on newly inserted element if i == (val - i) */
        hash.insert(i);
    }

    return false;
}

int main(void)
{
    assert(true == has_sum_of_2({1, 2, 2, 3}, 4));
    assert(true == has_sum_of_2({1, 2, -2, 3}, 0));
    assert(false == has_sum_of_2({1, 2, 2, 3}, 0));
    assert(false == has_sum_of_2({1}, 1));
}
