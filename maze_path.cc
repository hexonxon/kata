/**
 * Tell if there is any path between two points in a maze.
 */

#include <vector>
#include <cstdlib>

struct point {
    size_t x, y;
};

bool operator==(const point& p1, const point& p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

enum {
    CELL_EMPTY = 0,
    CELL_OCCUPIED,
    CELL_VISITED,
};

/*
 * Depth-first search
 */
bool find_path(std::vector<std::vector<int>>& maze, const point& start, const point& end)
{
    if (start == end) {
        return true;
    }

    if (maze[start.y][start.x] != CELL_EMPTY) {
        return false;
    }

    size_t width = maze[0].size();
    size_t height = maze.size();

    /* Visit cell */
    maze[start.y][start.x] = CELL_VISITED;

    /* Step right */
    if (start.x < width - 1) {
        if (find_path(maze, { start.x + 1, start.y }, end))
            return true;
    }

    /* Step left */
    if (start.x > 0) {
        if (find_path(maze, { start.x - 1, start.y }, end))
            return true;
    }

    /* Step up */
    if (start.y < height - 1) {
        if (find_path(maze, { start.x, start.y + 1}, end))
            return true;
    }

    /* Step down */
    if (start.y > 0) {
        if (find_path(maze, { start.x, start.y - 1}, end))
            return true;
    }

    return false;
}

#include <cassert>

int main(void)
{
    {
        std::vector<std::vector<int>> maze = {
            { CELL_EMPTY, CELL_OCCUPIED, CELL_EMPTY, CELL_EMPTY },
            { CELL_EMPTY, CELL_OCCUPIED, CELL_EMPTY, CELL_EMPTY },
            { CELL_EMPTY, CELL_OCCUPIED, CELL_EMPTY, CELL_EMPTY },
            { CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY },
        };

        assert(true == find_path(maze, {0, 0}, {3, 3}));
    }

    {
        /* Exit point is walled-off */
        std::vector<std::vector<int>> maze = {
            { CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY },
            { CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY },
            { CELL_EMPTY, CELL_EMPTY, CELL_OCCUPIED, CELL_OCCUPIED },
            { CELL_EMPTY, CELL_EMPTY, CELL_OCCUPIED, CELL_EMPTY },
        };

        assert(false == find_path(maze, {0, 0}, {3, 3}));
    }
}
