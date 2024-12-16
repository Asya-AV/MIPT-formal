#include <iostream>
#include <vector>
#include <unordered_set>
#include <map>

const int kmax_word_size = 10000000;
static const char kstart_terminal = '@';
const char* kepsela = "epsela"; //static

struct Rule {
    char rule_begin;
    std::string rule_end;
    
    Rule();
    Rule (char rule_begin,
        char rule_end) : rule_begin(rule_begin), rule_end(std::string({rule_end})) {}
    Rule (char rule_begin,
        std::string rule_end) : rule_begin(rule_begin), rule_end(rule_end) {}
};

struct Grammar{ // G = <N, Sigma, P, S>

    std::unordered_set<char> nonterm_set;
    std::unordered_set<char> term_set;
    std::map<char, std::vector<Rule>> rules;
    char start_nonterm;

    Grammar() = default;
    Grammar(std::unordered_set<char> nonterm_set,
            std::unordered_set<char> term_set,
            std::map<char, std::vector<Rule>> rules,
            char start_nonterm) : nonterm_set(nonterm_set), term_set(term_set), rules(rules), start_nonterm(start_nonterm) {}

    void print_grammar() {
        std::cout << "Nonterm: ";
        for (auto a : nonterm_set) std::cout << a << ' ';
        std::cout << '\n' << "Term: ";
        for (auto a : term_set) std::cout << a << ' ';
        std::cout << '\n' << "Rules: ";
        for (auto a : rules) {
            std::cout << a.first << ':';
            for (auto elem : a.second) std::cout << elem.rule_end << ' ';
            std::cout << '\n';
        }
        std::cout << "Start_nonterm: " << start_nonterm << '\n';
    }
};