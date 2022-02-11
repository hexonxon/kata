#include <vector>
#include <string>
#include <map>
#include <unordered_set>
#include <iostream>

struct graph
{
    std::map<std::string, std::vector<std::string>> adj_matrix;
};

static void topsort(const std::map<std::string, std::vector<std::string>>& graph,
                    std::unordered_set<std::string>& visited,
                    std::vector<std::string>& order,
                    const std::string& project)
{
    if (visited.find(project) != visited.end())
        return;
    
    visited.insert(project);
    for (const std::string& dep : graph.find(project)->second) {
        topsort(graph, visited, order, dep);
    }
    
    order.push_back(project);
}

std::vector<std::string> build_order(const std::vector<std::string>& projects, const std::vector<std::pair<std::string, std::string>>& deps)
{
    std::map<std::string, std::vector<std::string>> adj_matrix;
    
    for (const auto& p : projects) {
        adj_matrix[p] = {};
    }
    
    for (const auto& d : deps) {
        adj_matrix[d.second].push_back(d.first);
    }
    
    std::unordered_set<std::string> visited;
    std::vector<std::string> order;
    for (const auto& p : projects) {
        topsort(adj_matrix, visited, order, p);
    }
    
    return order;
}

int main()
{
    std::vector<std::string> projects = {"a", "b", "c", "d", "e", "f"};
    std::vector<std::pair<std::string, std::string>> deps = {
        { "a", "d" },
        { "f", "b" },
        { "b", "d" },
        { "f", "a" },
        { "d", "c" },
    };
    
    std::vector<std::string> order = build_order(projects, deps);
    for (const std::string& p : order) {
        std::cout << p << " ";
    }
    
    std::cout << std::endl;
    return 0;
}
