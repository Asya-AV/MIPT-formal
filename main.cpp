#include <iostream>
#include <string>

#include "source/cyk.h"

int main() {
    Reader reader;
    Grammar grammar = reader.read();
    Cyk cyk;
    size_t cnt_of_words;
    std::cin >> cnt_of_words;
    for (size_t i = 0; i < cnt_of_words; ++i) {
        std::string input_word;
        std::cin >> input_word;
        if (cyk.Predict(input_word, grammar)) {
            std::cout << "Yes\n";
        } else {
            std::cout << "No\n";
        }
    }
}