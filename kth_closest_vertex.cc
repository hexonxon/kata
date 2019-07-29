/*
 * Given a list of points as [x,y] pairs; a vertex in [x,y] form; and an integer k, return the kth closest points in terms of euclidean distance
 */

#include <vector>
#include <queue>
#include <stdexcept>
#include <utility>
#include <cassert>

using namespace std;

struct vertex
{
    int x;
    int y;
};

unsigned long calc_distance_squared(const vertex& a, const vertex& b)
{
    unsigned long x_dist = abs(a.x - b.x);
    unsigned long y_dist = abs(a.y - b.y);

    return x_dist * x_dist + y_dist * y_dist;
}

// nlogk solution with k-sized maxheap
vertex kth_closest_vertex(const vector<vertex>& v, const vertex& origin, size_t k)
{
    if (k == 0 || k > v.size()) {
        throw invalid_argument("bad k value");
    }

    typedef pair<const vertex*, unsigned long> vertex_dist;
    auto dist_cmp = [&](const vertex_dist& a, const vertex_dist& b) { return a.second < b.second; };

    // Build a k-sized maxheap of squared distances from origin to each vertex
    priority_queue<vertex_dist, vector<vertex_dist>, decltype(dist_cmp)> dist_heap(dist_cmp);
    for (const auto& i : v) {
        dist_heap.push(make_pair(&i, calc_distance_squared(i, origin)));
        if (dist_heap.size() > k) {
            dist_heap.pop();
        }
    }

    // Our kth closest element is at the top of the minheap
    return *(dist_heap.top().first);
}

int main(void)
{
    vector<vertex> v = { 
        { 0, 0 },
        { 0, 4 },
        { 4, 4 },
        { 4, 0 }
    };

    vertex origin = {1, 0};
    vertex closest;

    closest = kth_closest_vertex(v, origin, 1);
    assert(closest.x == 0 && closest.y == 0);

    closest = kth_closest_vertex(v, origin, 2);
    assert(closest.x == 4 && closest.y == 0);

    closest = kth_closest_vertex(v, origin, 3);
    assert(closest.x == 0 && closest.y == 4);

    closest = kth_closest_vertex(v, origin, 4);
    assert(closest.x == 4 && closest.y == 4);
}
