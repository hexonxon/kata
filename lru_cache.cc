/*
 * Basic LRU cache using basic data structures
 */

#include <list>
#include <algorithm>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <cstdlib>
#include <cassert>

template < typename object_type >
class lru_cache
{
public:

    explicit lru_cache(size_t capacity) : m_capacity(capacity), m_count(0)
    {
    }

    void fetch(object_type obj)
    {
        auto pos = m_lookup.find(obj);
        if (pos != m_lookup.end()) {
            m_cache.splice(m_cache.begin(), m_cache, pos->second);
            m_lookup[obj] = m_cache.begin();
        } else {
            if (m_capacity == m_count) {
                object_type lru = m_cache.back();
                m_cache.pop_back();
                m_lookup.erase(lru);
                --m_count;
            }

            m_cache.push_front(obj);
            m_lookup[obj] = m_cache.begin();
            ++m_count;
        }
    }

    void walk(std::function<void (const object_type&)> cb) const
    {
        for (auto i : m_cache) {
            cb(i);
        }
    }

private:
    size_t m_capacity;
    size_t m_count;

    std::list<object_type> m_cache;
    std::unordered_map<object_type, typename std::list<object_type>::iterator> m_lookup;
};

template <typename T>
static void validate(const lru_cache<T>& lru, const std::vector<T>& vals)
{
    int pos = 0;
    lru.walk([&](const T& obj) {
        assert(vals[pos++] == obj);
    });
}

int main(void)
{
    lru_cache<unsigned long long> lru(4);
    lru.fetch(1);
    validate(lru, {1});

    lru.fetch(2);
    validate(lru, {2, 1});

    lru.fetch(3);
    validate(lru, {3, 2, 1});

    lru.fetch(4);
    validate(lru, {4, 3, 2, 1});

    lru.fetch(5);
    validate(lru, {5, 4, 3, 2});

    lru.fetch(2);
    validate(lru, {2, 5, 4, 3});

    lru.fetch(6);
    validate(lru, {6, 2, 5, 4});

    return 0;
}
