#include "parser.h"

struct Reader {
    Grammar read() {
        Parser parser;

        size_t nonterm_cnt;
        size_t term_cnt;
        size_t rule_cnt;
        char start_symb;
        std::unordered_set<char> nonterm_set;
        std::unordered_set<char> term_set;
        std::map<char, std::vector<Rule>> rules;
        std::string nonterm_symbols;
        std::string term_symbols;
        
        std::cin >> nonterm_cnt >> term_cnt >> rule_cnt;
        do {
            std::getline(std::cin, nonterm_symbols); 
        } while (nonterm_symbols.size() == 0);
        nonterm_set = parser.parse_terms(nonterm_cnt, nonterm_symbols, true);
        
        do {
            std::getline(std::cin, term_symbols); 
        } while (term_symbols.size() == 0);
        term_set = parser.parse_terms(term_cnt, term_symbols, false);

        for (size_t i = 0; i < rule_cnt; ++i) {
            std::string raw_rule;

            do {
                std::getline(std::cin, raw_rule); 
            } while (raw_rule.size() == 0);

            auto rule = parser.parse_rule(raw_rule);
            char nonterm_rule = rule.first;

            if (rule.second.rule_end.empty()) {
                rules[nonterm_rule].push_back({nonterm_rule, kepsela}); 
            } else {
                rules[nonterm_rule].push_back(rule.second);
            }
        }
        
        std::cin >> start_symb;
        Grammar grammar(nonterm_set, term_set, rules, start_symb);
        return grammar;
    }
        
};