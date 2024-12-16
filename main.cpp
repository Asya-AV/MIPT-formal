#include <iostream>
#include <string>

#include "source/earley.h"

int main() {
    Reader reader;
    Grammar grammar = reader.read();
    Earley early;

    size_t cnt_of_words;
    std::cin >> cnt_of_words;
    for (size_t i = 0; i < cnt_of_words; ++i) {
        std::string input_word;
        std::cin >> input_word;
        if (early.check(input_word, grammar)) {
            std::cout << "Yes\n";
        } else {
            std::cout << "No\n";
        }
    }
}