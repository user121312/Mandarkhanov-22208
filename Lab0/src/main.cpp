#include "word_count.h"

int main(int argc, char* argv[]) {
    if (argc > 2) {
        WordCount::Counter obj(argv[1], argv[2]);
        obj.MakeCalc();
    }
    return 0;
}