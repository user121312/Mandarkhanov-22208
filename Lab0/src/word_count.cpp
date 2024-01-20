#include "word_count.h"

namespace WordCount {
    bool Counter::WrongSym(const char& s) const{
        if (s == ',' || s == ' ' || s == '.' || s == '\n'
        || s == '!' || s == '?' || s == '-' || s == '`') {
            return true;
        }
        return false;
    };
    Counter::Counter(const std::string& in, const std::string& out) {
        txt = in;
        csv = out;
    }
    void Counter::MakeCalc() {
        using namespace std;
        ifstream ifile(txt);
        ofstream ofile(csv);
        if (ifile.is_open() && ofile.is_open()) {
            size_t count = 0;
            string text;
            while (getline(ifile, text)) {
                for (size_t i = 0; i < text.size(); ++i) {
                    size_t j = i;
                    for (;j < text.size() && !WrongSym(text[j]); ++j);
                    if (j != i) {
                        ++count;
                        string word;
                        word.push_back(tolower(text[i]));
                        ++data[word + text.substr(i + 1, j - i - 1)];
                        i = j;
                    }
                }
            }
            vector<pair<string, int>> sorteddata(data.begin(), data.end());
            sort(sorteddata.begin(), sorteddata.end(), 
            [](const auto& p1, const auto& p2){
                if (p1.second != p2.second) {
                    return p1.second > p2.second;
                }
                return p1.first < p2.first;
            });
            ofile << "Word,Quantity,Percentage\n";
            for (const auto& i: sorteddata) {
                ofile << i.first << "," << i.second << "," << 
                (float) 100 * i.second / count << "%" << "\n";
            }
        }    
    }
}