#include <unordered_set>
#include <vector>
#include <functional>
#include <cassert>
#include <iostream>

struct graph {
    std::vector<std::vector<int>> adj_matrix;
};

bool dfs(const struct graph* g, int node, std::unordered_set<int>& visited, const std::function<bool(int)>& visitor)
{
    if (visitor(node))
        return true;
    
    visited.insert(node);
    for (int i : g->adj_matrix[node]) {
        if (visited.find(i) == visited.end())
            if (dfs(g, i, visited, visitor))
                return true;
    }
    
    return false;
}

bool has_path(const struct graph* g, int a, int b)
{
    if (!g)
        return false;
        
    /* Run DFS looking for nodes */
    std::unordered_set<int> visited;
    return dfs(g, a, visited, [&](int node) { return node == b; });
}

int main()
{
    struct graph g = {
        .adj_matrix = {
            {1, 2},
            {},
            {3, 4},
            {},
            {},
        },
    };
    
    assert(has_path(&g, 0, 0));
    assert(has_path(&g, 0, 1));
    assert(has_path(&g, 0, 3));
    assert(has_path(&g, 0, 4));
    assert(has_path(&g, 2, 3));
    assert(has_path(&g, 2, 4));

    assert(!has_path(&g, 1, 4));
    assert(!has_path(&g, 3, 4));
    
    return 0;
}
