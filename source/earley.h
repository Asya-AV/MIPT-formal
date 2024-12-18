#include "reader.h"

struct State {
    Rule rule;
    int point_index;
    int index;

    State() = default;
    State(const Rule& rule, int point_index, int index) : 
                    rule(rule), point_index(point_index), index(index) {}

    bool operator==(const State& other) const = default;

    struct Hash {
        size_t operator()(const State& state) const {
            size_t constant_1 = 1'000;
            size_t constant_2 = 1'000'000;
            return std::hash<char>{}(state.rule.rule_begin) +
                    std::hash<std::string>{}(state.rule.rule_end) * constant_1 + state.index * constant_2;
        }
    };
  };

  struct Layer {
    std::unordered_set<State, State::Hash> states;
    std::map<char, std::vector<State>> states_nonterm; //states with dot before nonterminal
    std::vector<State> ordered_states;
    size_t index_nocomplete;
    size_t index_nopredict;
    bool is_changed;

    Layer(): index_nocomplete(0), index_nopredict(0), is_changed(false) {}
    
    void Add(const State& state, const Grammar& cur_grammar) {
        if (states.find(state) != states.end()) {
            return;
        }
        states.insert(state);
        ordered_states.push_back(state);
        is_changed = true;
        char noterm = state.rule.rule_end[state.point_index];
        if (cur_grammar.nonterm_set.find(noterm) == cur_grammar.nonterm_set.end()) {
            return;
        }
        states_nonterm[noterm].push_back(state);
    }
};


class Earley {
    Grammar grammar;
    std::vector<Layer> layers;

    void Scan(int layer_number, const std::string& input_string) {
        for (auto state : layers[layer_number].states) {
            int dot_pos = state.point_index;
            if (dot_pos == state.rule.rule_end.size() || layer_number == layers.size() - 1) {
                continue;
            }
            
            char cur_latter = state.rule.rule_end[dot_pos];
            if (grammar.term_set.find(cur_latter) != grammar.term_set.end()) {
                if (cur_latter != input_string[layer_number]) {
                    continue;
                }
                
                State new_state = state;
                new_state.point_index += 1;
                layers[layer_number + 1].Add(new_state, grammar);
            }
        }
    }

    void Predict(int layer_number, const std::string& input_string) {
        Layer& layer = layers[layer_number];
        std::vector<State>& states = layer.ordered_states;

        for (size_t i = layer.index_nopredict; i < states.size(); ++i) {
            int dot_pos = states[i].point_index;
            if (dot_pos == states[i].rule.rule_end.size()) {
                continue;
            }
            int curr_noterm = states[i].rule.rule_end[dot_pos];
            if(grammar.nonterm_set.find(curr_noterm) != grammar.nonterm_set.end()) {
                for (auto rule : grammar.rules[curr_noterm]) {
                    State new_state(Rule(curr_noterm, rule.rule_end), 0, layer_number);
                    layer.Add(new_state, grammar);
                }
            }
        }
        layer.index_nopredict = states.size();
    }

    void Complete(int layer_number, const std::string& input_string) {
        Layer& layer = layers[layer_number];
        std::vector<State>& states = layer.ordered_states;

        for (size_t i = layer.index_nocomplete; i < states.size(); ++i) {
            int dot_pos = states[i].point_index;
            if (dot_pos != states[i].rule.rule_end.size()) {
                continue;
            }

            char noterm = states[i].rule.rule_begin;
            int l = states[i].index;
                for (auto old_state : layers[l].states_nonterm[noterm]) {
                State new_state = old_state;
                new_state.point_index += 1;
                layer.Add(new_state, grammar);
            }
        }
        layer.index_nocomplete = states.size();
    }

public:

    Earley() = default;

    void Check(const std::string& input_string, Grammar cur_grammar) {
        grammar = cur_grammar;
        grammar.rules[kstart_terminal].push_back(Rule(kstart_terminal, std::string(1, grammar.start_nonterm)));
        int word_sz = input_string.size();
        layers = std::vector<Layer>(word_sz + 1);
        Rule start_rule({kstart_terminal, grammar.start_nonterm});
        State start_state(start_rule, 0, 0);

        layers[0].Add(start_state, grammar);
        for (size_t i = 0; i <= word_sz; ++i) {
            while(layers[i].is_changed) {
                layers[i].is_changed = false;
                Predict(i, input_string);
                Complete(i, input_string);
            }
            Scan(i, input_string);
        }

        State finish_state(Rule(kstart_terminal, std::string(1, grammar.start_nonterm)), 1, 0);
        if (layers[word_sz].states.find(finish_state) != layers[word_sz].states.end()) {
            std::cout << "Yes\n";
        } else {
            std::cout << "No\n";
        }
    }
};
