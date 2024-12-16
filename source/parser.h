#include <cctype>
#include <unordered_set>
#include <string>
#include <vector>

#include "grammar.h"

bool is_nonterm(char letter) {
    return (bool)std::isupper(letter);
}


struct Parser {
    std::unordered_set<char> parse_terms(size_t cnt, std::string str, bool is_nonterm_fl) {
        std::unordered_set<char> terms_set;
        for (auto letter : str) {
            if (is_nonterm(letter) != is_nonterm_fl) {
                throw("Incorrect input data!\n");
            }
            terms_set.insert(letter);
        }
        if (terms_set.size() != cnt) {
            throw("Incorrect input data!\n");
        }
        return terms_set;
    }

    std::pair<char, Rule> parse_rule(std::string rule) {
        size_t cur_cnt = 0;
        std::vector<std::string> ret_rule(2);
        size_t cur_ind = 0;
        size_t sz = rule.size();
        for (size_t i = 0; i < sz; ++i) {
            if (rule[i] == '-' && i + 1 < sz && rule[i+1] == '>') {
                ++i;
                cur_ind++;
                continue;
            }
            if (rule[i] != ' ') {
                if ((cur_ind > 1) || (cur_ind == 0 && !is_nonterm(rule[i]))) {
                    throw("Incorrect input data!\n");
                }
                ret_rule[cur_ind] += rule[i];
            }
        }
        return {ret_rule[0][0], {ret_rule[0][0], ret_rule[1]}};
    }
};