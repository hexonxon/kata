/**
 * Given a two-dimensional array, if any element within is zero, make its whole row and column zero.
 */

#include <set>
#include <vector>
#include <cstdint>
#include <cstring>
#include <cassert>

/* Scan 2d array and look for zeroes, but don't zero out immediately - we might skip more zeroes this way.
 * Instead put row/col indexes in a vector and then do another pass and zero everything out. */
template <size_t nrows, size_t ncols>
void zero_columns_rows(int data[nrows][ncols])
{
    std::set<size_t> rows;
    std::set<size_t> cols;

    for (int row = 0; row < nrows; ++row) {
        for (int col = 0; col < ncols; ++col) {
            if (data[row][col] == 0) {
                rows.insert(row);
                cols.insert(col);
            }
        }
    }

    for (int row : rows) {
        memset(data[row], 0, sizeof(data[0]));
    }

    for (int col : cols) {
        for (int row = 0; row < nrows; ++row) {
            data[row][col] = 0;
        }
    }
}

int main(void)
{
    {
        /* Nothing to zero */
        int data[2][2] = {{1, 2}, {3, 4}};
        zero_columns_rows<2, 2>(data);

        assert(data[0][0] == 1);
        assert(data[0][1] == 2);
        assert(data[1][0] == 3);
        assert(data[1][1] == 4);
    }

    {
        int data[2][2] = {{1, 2}, {0, 4}};
        zero_columns_rows<2, 2>(data);

        assert(data[0][0] == 0);
        assert(data[0][1] == 2);
        assert(data[1][0] == 0);
        assert(data[1][1] == 0);
    }

    {
        /* One row has 2 zeroes, both columns should be zeroed */
        int data[2][2] = {{0, 0}, {3, 4}};
        zero_columns_rows<2, 2>(data);

        assert(data[0][0] == 0);
        assert(data[0][1] == 0);
        assert(data[1][0] == 0);
        assert(data[1][1] == 0);
    }
}
