/*
 * Flood fill algorithm.
 *
 * Given a 2D screen, location of a pixel in the screen and a color,
 * replace color of the given pixel and all adjacent same colored pixels with the given color.
 */

#include <vector>
#include <assert.h>
#include <stdlib.h>

using namespace std;

struct surface
{
    vector<vector<int>> lines;

    surface(size_t width, size_t height, int color)
    {
        for (size_t i = 0; i < height; ++i) {
            lines.push_back(vector<int>(width, color));
        }
    }

    int get_color(size_t x, size_t y) const
    {
        return lines[y][x];
    }

    void set_color(size_t x, size_t y, int color)
    {
        lines[y][x] = color;
    }

    size_t width() const
    {
        return lines[0].size();
    }

    size_t height() const
    {
        return lines.size();
    }

    /*
    void blit(fb_dev...) {
        ...
    }
    */
};

static void do_flood_fill(surface& s, size_t x, size_t y, int prev_color, int new_color)
{
    int color = s.get_color(x, y);
    if (color != prev_color) {
        return;
    }

    s.set_color(x, y, new_color);

    if (y + 1 < s.height()) {
        do_flood_fill(s, x, y + 1, prev_color, new_color);
    }

    if (y > 0) {
        do_flood_fill(s, x, y - 1, prev_color, new_color);
    }

    if (x + 1 < s.width()) {
        do_flood_fill(s, x + 1, y, prev_color, new_color);
    }

    if (x > 0) {
        do_flood_fill(s, x - 1, y, prev_color, new_color);
    }
}

static void flood_fill(surface& s, size_t x, size_t y, int color)
{
    if (x >= s.width() || y >= s.height()) {
        return;
    }

    int prev_color = s.get_color(x, y);
    if (prev_color == color) {
        return;
    }

    do_flood_fill(s, x, y, prev_color, color);
}

static bool is_filled(const surface& s, int color)
{
    for (size_t y = 0; y < s.height(); ++y) {
        for (size_t x = 0; x < s.width(); ++x) {
            if (s.get_color(x, y) != color) {
                return false;
            }
        }
    }

    return true;
}

int main(void)
{
    {
        surface s(10, 10, 0);
        flood_fill(s, 0, 0, 1);
        assert(is_filled(s, 1));
        flood_fill(s, 9, 9, 1);
        assert(is_filled(s, 1));
        flood_fill(s, 9, 9, 0);
        assert(is_filled(s, 0));
    }

    {
        surface s(8, 8, 0);
        s.lines[0] = {1, 1, 1, 1, 1, 1, 1, 1};
        s.lines[1] = {1, 1, 1, 1, 1, 1, 0, 0};
        s.lines[2] = {1, 0, 0, 1, 1, 0, 1, 1};
        s.lines[3] = {1, 2, 2, 2, 2, 0, 1, 0};
        s.lines[4] = {1, 1, 1, 2, 2, 0, 1, 0};
        s.lines[5] = {1, 1, 1, 2, 2, 2, 2, 0};
        s.lines[6] = {1, 1, 1, 1, 1, 2, 1, 1};
        s.lines[7] = {1, 1, 1, 1, 1, 2, 2, 1};

        flood_fill(s, 4, 4, 3);
        
        assert(s.lines[0] == vector<int>({1, 1, 1, 1, 1, 1, 1, 1}));
        assert(s.lines[1] == vector<int>({1, 1, 1, 1, 1, 1, 0, 0}));
        assert(s.lines[2] == vector<int>({1, 0, 0, 1, 1, 0, 1, 1}));
        assert(s.lines[3] == vector<int>({1, 3, 3, 3, 3, 0, 1, 0}));
        assert(s.lines[4] == vector<int>({1, 1, 1, 3, 3, 0, 1, 0}));
        assert(s.lines[5] == vector<int>({1, 1, 1, 3, 3, 3, 3, 0}));
        assert(s.lines[6] == vector<int>({1, 1, 1, 1, 1, 3, 1, 1}));
        assert(s.lines[7] == vector<int>({1, 1, 1, 1, 1, 3, 3, 1}));
    }

    return 0;
}
