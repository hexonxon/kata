/**
 * You are given an array of positive numbers from 1 to n,
 * such that all numbers from 1 to n are present except one number 'x'.
 * We have to find 'x'. The input array is not sorted.
 */

#include <cassert>
#include <cstdint>

#include <vector>
#include <numeric>

int find_missing(const std::vector<int>& data, size_t n)
{
    assert(data.size() == n - 1);
 
    int expected_sum = n * (n + 1) / 2;
    int sum = std::accumulate(data.begin(), data.end(), 0);

    return expected_sum - sum;
}

int main(void)
{
    assert(7 == find_missing({1, 2, 3, 4, 5, 6, 8, 9, 10}, 10));
    return 0;
}
