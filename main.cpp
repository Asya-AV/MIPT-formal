#include "source/earley.h"

int main() {
    Earley earley;
    Reader reader;
    Grammar grammar = reader.Parse();

    size_t cnt_of_words;
    std::cin >> cnt_of_words;

    for (int i = 0; i < cnt_of_words; ++i) {
        std::string word;
        std::cin >> word;
        earley.Check(word, grammar);
    }
    return 0;
}