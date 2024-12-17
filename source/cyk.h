#include <string>
#include <map>

#include "reader.h"


void print_rule(Rule rule) {
    std::cout << rule.rule_begin << "->" <<  rule.rule_end << ' ';
}

struct Cyk {
    std::vector<std::vector<std::vector<bool>>> dp;
    std::string word;
    size_t word_sz = 0;

    size_t get_hash(char nonterm) {
        return (nonterm - 'A');
    }

    void init(std::string input_string, Grammar grammar) {
        word = input_string;
        word_sz = input_string.size();
        size_t nont_sz = get_hash('Z') + 1;
        dp.resize(nont_sz);
        std::vector<bool> mini_mas;
        mini_mas.resize(word_sz);
        std::vector<std::vector<bool>> mas;
        mas.resize(word_sz);
        for (size_t i = 0; i < word_sz; ++i) mas[i] = mini_mas;
        for (size_t i = 0; i < nont_sz; ++i) dp[i] = mas;
    }


    bool Predict(std::string input_string, Grammar grammar) {
        init(input_string, grammar);
        for (int j = 0; j < word_sz; ++j) {
            for (auto full_rule : grammar.rules) {
                for (Rule rule : grammar.rules[full_rule.first]) {
                    if (rule.rule_end.size() == 1) {
                        if (rule.rule_end == std::string({input_string[j]})) {
                            dp[get_hash(rule.rule_begin)][j][j] = true;
                        }
                    }   
                } 
            }
            for (int i = j; i >= 0; --i) {
                for (int k = i; k < j; ++k) {
                    for (auto full_rule : grammar.rules) {
                        for (Rule rule : grammar.rules[full_rule.first]) {
                            if (rule.rule_end.size() == 2) {
                                if (dp[get_hash(rule.rule_end[0])][i][k] && dp[get_hash(rule.rule_end[1])][k+1][j]) {
                                    dp[get_hash(rule.rule_begin)][i][j] = true;
                                }
                            }
                        }        
                    }
                }
            }
        }
        return dp[get_hash(grammar.start_nonterm)][0][word_sz-1];
    }
};