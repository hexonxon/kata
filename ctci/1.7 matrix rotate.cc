#include <iostream>
#include <assert.h>
#include <string.h>

struct matrix2 {
    size_t n;
    uint32_t* data;
};

static inline uint32_t get(const struct matrix2* m, size_t r, size_t c)
{
    return m->data[m->n * r + c];
}

static inline void set(struct matrix2* m, size_t r, size_t c, uint32_t v)
{
    m->data[m->n * r + c] = v;
}

static void dump(const struct matrix2* m)
{
    for (size_t i = 0; i < m->n; ++i) {
        for (size_t j = 0; j < m->n; ++j) {
            std::cout << get(m, i, j) << " ";
        }

        std::cout << std::endl;
    }
}

void mrotate90(struct matrix2* m)
{
    size_t first = 0;
    size_t last = m->n - 1;
    size_t dim = m->n;
    while (dim > 1) {
        for (size_t i = 0; i < dim - 1; ++i) {
            uint32_t temp = get(m, first, first + i);
            set(m, first, first + i, get(m, last - i, first));
            set(m, last - i, first, get(m, last, last - i));
            set(m, last, last - i, get(m, first + i, last));
            set(m, first + i, last, temp);
        }

        dim >>= 1;
        first++;
        last--;
    }
}

int main() {
    uint32_t data[] = {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16,
    };
  
    struct matrix2 m = {
        .n = 4,
        .data = data,
    };

    dump(&m);
    mrotate90(&m);
    dump(&m);
    
    return 0;
}
