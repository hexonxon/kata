/**
 * 1.8 Zero Matrix: Write an algorithm such that if an element in an MxN matrix is 0, its entire row and
 * column are set to 0
 */

#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <vector>
#include <iostream>

struct matrix2d {
    size_t nrows;
    size_t ncols;
    uint32_t* data;
};

static uint32_t get(const struct matrix2d* m, size_t r, size_t c)
{
    assert(m);
    assert(r < m->nrows);
    assert(c < m->ncols);
    return m->data[r * m->ncols + c];
}

static void set(struct matrix2d* m, size_t r, size_t c, uint32_t v)
{
    assert(m);
    assert(r < m->nrows);
    assert(c < m->ncols);
    m->data[r * m->ncols + c] = v;
}

static void dump(const struct matrix2d* m)
{
    assert(m);
    for (size_t r = 0; r < m->nrows; ++r) {
        for (size_t c = 0; c < m->ncols; ++c) {
            std::cout << get(m, r, c) << " ";
        }
        std::cout << std::endl;
    }
}

static void zero_row(struct matrix2d* m, size_t r)
{
    assert(m);
    assert(r < m->nrows);
    memset(m->data + r * m->ncols, 0, sizeof(*m->data) * m->ncols);
}

static void zero_col(struct matrix2d* m, size_t c)
{
    assert(m);
    assert(c < m->ncols);
    for (size_t r = 0; r < m->nrows; ++r) {
        set(m, r, c, 0);
    }
}

void zero_matrix(struct matrix2d* m)
{
    if (!m)
        return;
        
    std::vector<size_t> zero_rows;
    std::vector<size_t> zero_cols;
    
    for (size_t r = 0; r < m->nrows; ++r) {
        for (size_t c = 0; c < m->ncols; ++c) {
            if (0 == get(m, r, c)) {
                zero_rows.push_back(r);
                zero_cols.push_back(c);
            }
        }
    }
    
    for (size_t r : zero_rows)
        zero_row(m, r);

    for (size_t c : zero_cols)
        zero_col(m, c);
}

int main()
{
    uint32_t data[] = {
        1, 2, 3, 4,
        9, 0, 8, 0,
        5, 6, 7, 0,
    };
    
    struct matrix2d m = {
        .nrows = 3,
        
        .data = data,
    };
    
    dump(&m);
    zero_matrix(&m);
    dump(&m);
    
    return 0;
}
