#include "parser.h"

Grammar do_chnf(Grammar grammar) {
    char next_free_nonterm = 33;
    std::map<char, std::vector<Rule>> new_rules;
    for (auto full_rule : grammar.rules) {
        for (Rule rule : grammar.rules[full_rule.first]) {
            int rule_sz = rule.rule_end.size();
            if (rule.rule_end != kepsela && rule_sz > 2) {
                std::string new_rule_end = "";
                new_rule_end += rule.rule_end[rule_sz - 2] + rule.rule_end[rule_sz - 1];
                new_rules[next_free_nonterm].push_back({next_free_nonterm, new_rule_end});
                grammar.nonterm_set.insert(next_free_nonterm);
                ++next_free_nonterm;
                for (int i = rule_sz - 3; i > 0; --i) {
                    new_rule_end = "" + rule.rule_end[i] + (char)(next_free_nonterm - 1);
                    new_rules[next_free_nonterm].push_back({next_free_nonterm, new_rule_end});
                    grammar.nonterm_set.insert(next_free_nonterm);
                    ++next_free_nonterm;
                }
                new_rule_end = "" + rule.rule_end[0] + (char)(next_free_nonterm - 1);
                new_rules[rule.rule_begin].push_back({rule.rule_begin, new_rule_end});
            }
        }
    }
    Grammar new_grammar(grammar.nonterm_set, grammar.term_set, new_rules, grammar.start_nonterm);
    return new_grammar;
}

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
        std::unordered_set<char> generating_symb;
        
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
                generating_symb.insert(nonterm_rule);
            } else {
                rules[nonterm_rule].push_back(rule.second);
                bool have_nonterm = false;
                for (auto c : rule.second.rule_end) {
                    if (is_nonterm(c)) {
                        have_nonterm = true;
                        break;
                    }
                }
                if (!have_nonterm) {
                    generating_symb.insert(nonterm_rule);
                }
            }
        }
        std::cin >> start_symb;
        Grammar grammar(nonterm_set, term_set, rules, start_symb);
        Grammar grammar_in_chnf = do_chnf(grammar);
        return grammar_in_chnf;
    }
        
};