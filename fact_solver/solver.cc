#include "rule.h"
#include "facts.h"

#include <cstring>
#include <cstdlib>
#include <cassert>
#include <cstdint>

#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <regex>

using namespace solver;
using namespace std;

typedef shared_ptr<IRule> RulePtr;

static void parse(ifstream& input, FactSet& facts, std::vector<RulePtr>& rules)
{
    string line;

    regex csv("\"(.*)\"(\\s*,\\s*\"(.*)\")*");
    smatch match;

    // Line 1 is a list of known facts separated by a comma: "A", "B"
    getline(input, line);
    if (regex_search(line, match, csv)) 
    {
        // Iterate through all matched rule occurences
        string res = match[0];
        regex subr("\"([A-Z_])\"");

		sregex_iterator i(res.begin(), res.end(), subr);
		sregex_iterator end;

        while (i != end) {
            match = *i++;
            facts.add(match[1]);
        }
    }

    // Line 2 is a list of rules separated by a comma
    getline(input, line);
    if (regex_search(line, match, csv)) 
    {
        string res = match[0];

        // match for identity rule
        {
            regex identity("\"([A-Z_])\\s*->\\s*([A-Z_])\"");
	    	sregex_iterator i(res.begin(), res.end(), identity);
		    sregex_iterator end;
            while (i != end) {
                match = *i++;
                rules.push_back( make_shared<IdentityRule>(match[1], match[2]) );
            }
        }

        // match for binary rules
        {
            regex binary("\"([A-Z_])\\s*([|,])\\s*([A-Z_])\\s*->\\s*([A-Z_])\"");
	    	sregex_iterator i(res.begin(), res.end(), binary);
		    sregex_iterator end;
            while (i != end) {
                match = *i++;
                if (match[2] == "|") {
                    rules.push_back( make_shared<DisjunctionRule>(match[1], match[3], match[4]) );
                } else if (match[2] == ",") {
                    rules.push_back( make_shared<ConjunctionRule>(match[1], match[3], match[4]) );
                } else {
                    assert(0);
                }
            }
        }
    }
}

// Derive facts from set of rules
static void derive(FactSet& facts, const std::vector<RulePtr>& rules)
{
    // Produce new facts
    bool new_facts;
    do {
        new_facts = false;
        for (auto i = rules.begin(); i != rules.end(); ++i) {
            std::string fact = (*i)->apply(facts);
            if (!fact.empty() && !facts.contains(fact)) {
                // If rule applies we have a potentially changed set of facts and need to re-apply previous rules again
                // Remember that and keep going
                facts.add(fact);
                new_facts = true;
            }
        }
    } while (new_facts);
}

static void dump(FactSet& facts)
{
    auto i = facts.get_all().begin();
    auto end = facts.get_all().end();

    if (i != end) {
        cout << *i;
    }

    while (++i != end) {
        cout << ", " << *i;
    }

    cout << endl;
}

int main(int argc, char** argv)
{
    FactSet facts;
    std::vector<RulePtr> rules;

    if (argc != 2) {
        std::cerr << "Please provide a file name" << std::endl;
        return EXIT_FAILURE;
    }

    ifstream input(argv[1]);
    if (!input.is_open()) {
        cerr << "Cant open file " << argv[1] << endl;
        return EXIT_FAILURE;
    }

    // 1. Parse input file
    parse(input, facts, rules);
    
    // 2. Derive new facts
    derive(facts, rules);

    // 3. Show results
    cout << "Derived facts:\n";
    dump(facts);

    // 4. PROFIT!!
    return 0;
}

