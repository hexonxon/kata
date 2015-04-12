#ifndef SOLVER_RULE_H
#define SOLVER_RULE_H

#include "facts.h"

namespace solver
{
    /**
     * @brief Polymorphic rule interface. 
     *
     * Each rule applies its logic on a set of existing facts producing a possibly new set of facts if rule applies.
     * Also, I rule!!.. get it?.. never mind..
     */
    class IRule
    {
    public:

        /**
         * @brief Given a set of facts test if this rule applies
         *
         * Returns true if rule applies and false if not.
         */
        virtual std::string apply(const FactSet& facts) const = 0;

        virtual ~IRule() {}
    };
        
    /**
     * @brief Identity rule in the form: "FACT1 -> FACT2"
     */
    class IdentityRule : public IRule
    {
    public:

        IdentityRule(const std::string& fact, const std::string& derived) : m_fact(fact), m_derived(derived) {}

        std::string apply(const FactSet& facts) const {
            if (facts.contains(m_fact)) 
                return m_derived;
            else 
                return "";
        }

    private:

        std::string m_fact;
        std::string m_derived;
    };

    /**
     * @brief Boolean AND rule
     */
    class ConjunctionRule : public IRule
    {
    public:
    
        ConjunctionRule(const std::string& lhv, const std::string& rhv, const std::string& derived) 
            : m_lhv(lhv)
            , m_rhv(rhv)
            , m_derived(derived) 
        {}

        std::string apply(const FactSet& facts) const {
            if (facts.contains(m_lhv) && facts.contains(m_rhv)) {
                return m_derived;
            } else {
                return "";
            }
        }

    private:

        std::string m_lhv;
        std::string m_rhv;
        std::string m_derived;
    };

    /**
     * @brief Boolean OR rule
     */
    class DisjunctionRule : public IRule
    {
    public:
        
        DisjunctionRule(const std::string& lhv, const std::string& rhv, const std::string& derived) 
            : m_lhv(lhv)
            , m_rhv(rhv)
            , m_derived(derived) 
        {}

        std::string apply(const FactSet& facts) const {
            if (facts.contains(m_lhv) || facts.contains(m_rhv)) {
                return m_derived;
            } else {
                return "";
            }
        }

    private:

        std::string m_lhv;
        std::string m_rhv;
        std::string m_derived;
    };
}

#endif

