#include <cstdlib>
#include <cassert>
#include <cstdint>

uint32_t intersection(uint32_t n, uint32_t m, size_t i, size_t j)
{
    assert(j > i);
    uint32_t mask = ~((1u << (j - i + 1)) - 1);
    return (n & (mask << i)) | (m << i);
}

int main()
{
    assert(intersection(0b10000000000, 0b10011, 2, 6) == 0b10001001100);
    assert(intersection(0b10011111100, 0b10011, 2, 6) == 0b10011001100);
    return 0;
}
