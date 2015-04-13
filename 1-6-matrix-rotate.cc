#include <vector>
#include <iostream>

#include <stddef.h>
#include <stdint.h>

using namespace std;

#define N 4

struct matrix
{
    uint32_t data[N][N];
};

static void init(matrix& m) 
{
    for (size_t x = 0; x < N; ++x) {
        for (size_t y = 0; y < N; ++y) {
            m.data[x][y] = x * N + y;
        }
    }
}

static void roll(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d)
{
    uint32_t tmp = a;
    a = b;
    b = c;
    c = d;
    d = tmp;
}

static void rotate(matrix& m)
{
    for (size_t i = 0; i < (N >> 1); ++i) 
    {
        for (size_t j = 0; j < ((N + 1) >> 1); ++j) 
        {
            roll(m.data[i][j], m.data[N - 1 - j][i], m.data[N - 1 - i][N - 1 - j], m.data[j][N - 1 - i]);
        }
    }
}

static void dump(const matrix& m) 
{
    for (size_t x = 0; x < N; ++x) {
        for (size_t y = 0; y < N; ++y) {
            cout << m.data[x][y] << " ";
        }

        cout << endl;
    }
}

int main(void)
{
    matrix m;

    cout << "Original: \n";

    init(m);
    dump(m);

    cout << "\nRotated: \n";

    rotate(m);
    dump(m);

    return 0;
}

