#include <string>
#include <map>
#include <set>

#include "reader.h"

struct State {
    Rule rule;
    size_t point_index = 0;
    size_t index;

    State() = default;
    State(Rule rule, 
        size_t point_index,
        size_t index) : rule(rule), point_index(point_index), index(index) {}
        
        bool operator< (const State& state) const {
            if (index != state.index) return index < state.index;
            if (point_index != state.point_index) return point_index < state.point_index;
            if (rule.rule_end != state.rule.rule_end) return rule.rule_end < state.rule.rule_end;
            return rule.rule_begin < state.rule.rule_begin;
        }
   

};


void print_rule(Rule rule) {
    std::cout << rule.rule_begin << "->" <<  rule.rule_end << ' ';
}

void print_state(State state, size_t j) {
    print_rule(state.rule);
    std::cout << state.point_index << ' ' << state.index << ' ' << j;
    std::cout << '\n';
}

struct Earley {
    std::vector<std::set<State>> state_set;
    std::string word;
    size_t word_sz = 0;

    void init(std::string input_string, Grammar cur_grammar) {
        word = input_string;
        word_sz = input_string.size();
        state_set.resize(word_sz + 1);
    }

    void Scan(State state, size_t j) {
        char next_letter = state.rule.rule_end[state.point_index];
        if (j < word_sz && word[j] == next_letter) {
            ++state.point_index;
            state_set[j+1].insert(state);
        }            
    }

    bool Predict(State state, size_t j, Grammar grammar) {
        bool is_change = false;
        char next_nonterm = state.rule.rule_end[state.point_index];
        for (Rule rule : grammar.rules[next_nonterm]) {
            State add_state({rule, 0, j});
            size_t set_sz = state_set[j].size();
            state_set[j].insert(add_state);
            if (set_sz != state_set[j].size()) {
                is_change = true;
            }
        }
        return is_change;
    }

    bool Complete(State state, size_t j) {
        bool is_change = false;
        size_t k = state.index;
        for (State state_k : state_set[k]) {
            if (state_k.rule.rule_end[state.point_index] != state.rule.rule_begin) continue;
            if (state_k.point_index == state_k.rule.rule_end.size()) continue;
            ++state_k.point_index;
            size_t set_sz = state_set[j].size();
            state_set[j].insert(state_k);
            if (set_sz != state_set[j].size()) {
                is_change = true;
            }
        }
        return is_change;
    }

    bool check(std::string input_string, Grammar grammar) {
        init(input_string, grammar); 
        Rule start_rule({kstart_terminal, grammar.start_nonterm});
        State start_state(start_rule, 0, 0);
        state_set[0].insert(start_state);

        for (size_t j = 0; j < word_sz; ++j) {
            bool comp_fl = true;
            bool pred_fl = true;
            while (comp_fl || pred_fl) {
                comp_fl = false;
                pred_fl = false;
                for (auto state : state_set[j]) {
                    if (state.rule.rule_end == kepsela) {
                        comp_fl = Complete(state, j);
                        continue;
                    }
                    if (state.point_index != state.rule.rule_end.size()) {
                        char next_letter = state.rule.rule_end[state.point_index];
                        if (is_nonterm(next_letter)) {
                            pred_fl = Predict(state, j, grammar);
                        } else {
                            Scan(state, j);
                        }
                    } else {
                        comp_fl = Complete(state, j);
                    }
                }
            }
        }
        State finish_state({kstart_terminal, grammar.start_nonterm}, 0, word_sz);
        return (state_set[word_sz].find(finish_state) != state_set[word_sz].end());
    }
};