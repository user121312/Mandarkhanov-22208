#pragma once
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

namespace WordCount {
    class Counter {
    private:
        std::string txt;
        std::string csv;
        std::map<std::string, int> data;
        bool WrongSym(const char&) const;
    public:
        Counter(const std::string&, const std::string&);
        void MakeCalc();
    };
}