#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include <vector>
#include <deque>
#include <utility>
#include <algorithm>
#include <stdexcept>

#ifndef countof
#   define countof(a) (sizeof(a) / sizeof(*a))
#endif

namespace binary_indexed_tree {

    /* O(log(data size)) */
    int sum(const std::vector<int>& data, int index)
    {
        int sum = 0;

        assert(index >= 0);

        while (index > 0) {
            sum += data[index];
            int delta = index & (-index);
            index -= delta;
        }

        return sum;
    }

    /* O(log(data size)) */
    void update(std::vector<int>& data, int index, int val)
    {
        assert(index >= 0);

        while (index < data.size()) {
            data[index] += val;
            int delta = index & (-index);
            index += delta;
        }
    }
}

static int move_pivot(std::vector<int>& tree, std::deque<int>& stack, std::vector<int>::iterator elem, int inv)
{
    /* Add left-most element to right tree */
    binary_indexed_tree::update(tree, *elem, 1);
    inv += binary_indexed_tree::sum(tree, *elem - 1) + 1; /* Count +1 inversion for implicit pivot swap */

    /* Move to previous left-most inversion count */
    stack.pop_back();

    return (stack.empty() ? 0 : stack.back()) + inv;
}

/* Calculate number of adjacent swaps to put sequence in order a0 > ... > ak-1 >  ak < ak + 1 ... < an
   Sequence does not contain repeating values. */
static int n_swaps(int *a, size_t size)
{
    std::vector<int> seq(a, a + size);

    /* Find max, min and min pivot position */
    int min = INT_MAX;
    int max = 0;
    std::vector<int>::iterator pmin;
    for (std::vector<int>::iterator i = seq.begin(); i != seq.end(); ++i) {
        if (*i < min) {
            min = *i;
            pmin = i;
        }

        if (*i > max) {
            max = *i;
        }
    }

    /* Convert sequence to non-negative values */
    if (min < 0) {
        for (size_t i = 0; i < seq.size(); ++i) {
            seq[i] -= min;
        }

        max -= min;
    }

    int inversion_count = 0;

    /* Calculate history of inversion counts before pivot by gradually building binary indexed tree */
    std::deque<int> left_inversions;
    std::vector<int> left_tree(max + 1, 0);

    inversion_count = 0;
    for (std::vector<int>::iterator i = seq.begin(); i != pmin; ++i) {
        inversion_count += binary_indexed_tree::sum(left_tree, *i - 1);
        left_inversions.push_back(inversion_count);

        binary_indexed_tree::update(left_tree, *i, 1);
    }

    /* Calculate all consequitive inversion counts after pivot */
    std::deque<int> right_inversions;
    std::vector<int> right_tree(max + 1, 0);

    inversion_count = 0;
    for (std::vector<int>::iterator i = seq.end() - 1; i != pmin; --i) {
        inversion_count += binary_indexed_tree::sum(right_tree, *i - 1);
        right_inversions.push_back(inversion_count);

        binary_indexed_tree::update(right_tree, *i, 1);
    }

    int total_left_swaps = (left_inversions.empty() ? 0 : left_inversions.back());
    int total_right_swaps = (right_inversions.empty() ? 0 : right_inversions.back());

    int min_swaps = total_left_swaps + total_right_swaps;
    if (min_swaps == 0) {
        return 0;
    }

    /* Move pivot to the left and update inversion counts. */
    inversion_count = total_right_swaps;
    for (std::vector<int>::iterator i = pmin; i != seq.begin(); --i) {
        int nswaps = move_pivot(right_tree, left_inversions, i - 1, inversion_count);
        if (nswaps < min_swaps) {
            min_swaps = nswaps;
            if (min_swaps == 0) {
                return 0;
            }
        }
    }

    /* Move pivot to the right and update inversion counts. */
    inversion_count = total_left_swaps;
    for (std::vector<int>::iterator i = pmin; i != seq.end() - 1; ++i) {
        int nswaps = move_pivot(left_tree, right_inversions, i + 1, inversion_count);
        if (nswaps < min_swaps) {
            min_swaps = nswaps;
            if (min_swaps == 0) {
                return 0;
            }
        }
    }

    return min_swaps;
}


int main(void)
{
    {
        int vals[] = {6, 0, 5, 2, 3, 4};
        assert(n_swaps(vals, countof(vals)) == 1);
    }

    {
        int vals[] = {6, -10, -5, 2, 3, 4};
        assert(n_swaps(vals, countof(vals)) == 0);
    }

    {
        int vals[] = {0, 2, 3, 4};
        assert(n_swaps(vals, countof(vals)) == 0);
    }

    {
        int vals[] = {4, 3, 2, 0};
        assert(n_swaps(vals, countof(vals)) == 0);
    }
}
