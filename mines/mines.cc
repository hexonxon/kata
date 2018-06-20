// g++ -std=c++11 mines.cc

#include <cstdlib>
#include <cstring>
#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <fstream>
#include <boost/algorithm/string.hpp>

using namespace std;

// Describes parsed mine field
struct field_t {
    // Field lines, true means this slot has a mine
    vector<vector<bool>> lines;

    size_t total_lines() const {
        return lines.size();
    }

    const vector<bool>& get_line(size_t num) const {
        return lines[num];
    }

    bool has_mine(size_t line, size_t pos) const {
        return lines[lines][pos];
    }
};

// Parse a single input line
static vector<bool> parse_line(const string& line)
{
    vector<bool> res;

    // Tokenize string with boost
    // It's an overkill for this type of task, but i was given 15 minutes, so overkill it is
    vector<string> tokens;
    boost::split(tokens, line, boost::is_space());

    // Check that all tokens are valid
    for (auto str : tokens) {
        if (str == "X") {
            res.push_back(true);
        } else if (str == "O") {
            res.push_back(false);
        } else {
            throw invalid_argument("Invalid token");
        }
    }

    // Assume we're working with c++11 move semantics
    return res;
}

static field_t parse_mine_field(ifstream& input)
{
    field_t field;

    string str;
    while (!getline(input, str).eof()) {
        vector<bool> line = parse_line(str);

        // skip empty lines
        if (str.size() == 0) {
            continue;
        }

        if (field.total_lines() > 0) {
            // If this is not the first line, double check that all lines have the same length
            if (field.get_line(0).size() != line.size()) {
                throw invalid_argument("Bad line size");
            }

            // Task also specifies an NxN field, so check if we're not overflowing line count
            if (field.get_line(0).size() == field.total_lines()) {
                throw invalid_argument("Field is not NxN");
            }
        }

        field.lines.push_back(std::move(line));
    }

    // Assume we're working with c++11 move semantics
    return field;
}

// Count neighbour mines in a line
static unsigned int count_neighbour_mines_in_line(const vector<bool>& line, size_t pos)
{
    assert(pos < line.size());

    unsigned int res = line[pos];
    if (pos > 0) {
        res += line[pos - 1];
    }

    if ((pos + 1) < line.size()) {
        res += line[pos + 1];
    }

    return res;
}

static unsigned int count_adjacent_mines(const field_t& field, size_t line, size_t pos)
{
    assert(line < field.lines.size());

    unsigned int res = count_neighbour_mines_in_line(field.get_line(line), pos);
    if (line > 0) {
        res += count_neighbour_mines_in_line(field.get_line(line - 1), pos);
    }

    if ((line + 1) < field.lines.size()) {
        res += count_neighbour_mines_in_line(field.get_line(line + 1), pos);
    }

    return res;
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "Provide a file name" << std::endl;
        return EXIT_FAILURE;
    }

     ifstream input(argv[1]);
     if (!input.is_open()) {
         cerr << "Cant open file " << argv[1] << endl;
         return EXIT_FAILURE;
     }

     field_t field = parse_mine_field(input);

     for (size_t i = 0; i < field.total_lines(); ++i) {
         for (size_t j = 0; j < field.get_line(i).size(); ++j) {
            if (field.has_mine(i, j)) {
                cout << "X ";
            } else {
                cout << count_adjacent_mines(field, i, j) << " ";
            }
         }

         cout << endl;
     }

     return EXIT_SUCCESS;
}
