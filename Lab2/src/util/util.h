#pragma once
#include <vector>
#include <string>

struct Point {
    int x;
    int y;
};

const std::string Default_name = "Conway's game of life";
const std::string Default_b_rule = "3";
const std::string Default_s_rule = "23";
const std::vector<Point> Default_frame = {
    {0, 0}, {0, 1}, {1, 1}, {2, 1}, {2, 3}, {2, 4}, 
    {3, 2}, {4, 9}, {5, 4}, {5, 8}, {5, 9}, {6, 6}, 
    {7, 6}, {7, 7}, {7, 8}, {12, 11}, {12, 12}, {12, 13}, 
    {13, 13}, {14, 10}, {14, 11}, {14, 15}, {15, 10}, {16, 17}, 
    {17, 15}, {17, 16}, {17, 18}, {18, 18}, {19, 18}, {19, 19}
};

class Field {
private:
    int size_ = 30;
    std::vector<unsigned char> field_;
public:
    Field();
    Field(std::vector<Point> coordinates);
    Field(const Field& other);
    Field& operator=(const Field& other);

    int get_size() const;
    bool get_state(int row, int column) const;
    void set_state(int row, int column, bool value);
};