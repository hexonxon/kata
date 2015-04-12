#ifndef SOLVER_FACTS_H
#define SOLVER_FACTS_H

#include <string>
#include <set>

namespace solver
{
    /**
     * @brief A bag of known facts
     *
     * TODO: I can use a hash table to accelerate fact lookup
     */
    class FactSet
    {
    public:

        typedef std::set<std::string> set_type;

        bool contains(const std::string& fact) const {
            return m_facts.end() != m_facts.find(fact);
        }

        void add(const std::string& fact) {
            m_facts.insert(fact);
        }

        const set_type& get_all() const {
            return m_facts;
        }

    private:

        set_type m_facts;
    };
}

#endif 

