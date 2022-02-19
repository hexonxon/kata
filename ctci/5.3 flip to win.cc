#include <cstdlib>
#include <cassert>
#include <cstdint>
#include <algorithm>

size_t flip_to_win(uint32_t n)
{
    if (n == 0) {
        return 1;
    }
    
    size_t longest = 0;
    size_t running = 0;
    bool flipped = false;
    size_t flip_pos = 0;
    for (size_t i = 0; i < 32; ++i) {
        if ((n & (1u << i)) == 0) {
            if (flipped) {
                longest = std::max(running, longest);
                running = 0;
                i = flip_pos;
            } else {
                flipped = true;
                flip_pos = i;
                ++running;
            }
        } else {
            ++running;
        }
    }

    return std::max(running, longest);
}

int main()
{
    assert(flip_to_win(0) == 1);
    assert(flip_to_win(0b11011101111) == 8);
    assert(flip_to_win(0b11011101110) == 7);
    assert(flip_to_win(0b11011001110) == 5);
    return 0;
}
