/**
 * Find all sum combinations
 *
 * Given a positive integer, target, print all possible combinations of positive integers that sum up to the target number.
 */

#include <vector>

void reduce_sum(int target, int start, std::vector<int>& partial, std::vector<std::vector<int>>& all_sums)
{
    if (target == 0 && partial.size() > 1) {
        all_sums.push_back(partial);
    }

    while (start <= target) {
        partial.push_back(start);
        reduce_sum(target - start, start, partial, all_sums);
        partial.pop_back();
        ++start;
    }
}

std::vector<std::vector<int>> reduce_sum(int target)
{
    std::vector<std::vector<int>> all_sums;
    std::vector<int> partial_sum = {};
    reduce_sum(target, 1, partial_sum, all_sums);

    return all_sums;
}

#include <iostream>

int main(void)
{
    for (const auto& v : reduce_sum(5)) {
        for (int i : v) {
            std::cout << " " << i;
        }
        std::cout << std::endl;
    }
}
