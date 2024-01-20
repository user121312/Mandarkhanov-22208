#include "util.h"
#include <iostream>

namespace {
    int normalize(int coord, int size) {
        int koef = coord / size;
        koef -= coord < (-1) * size? 1 : 0;
        if (coord >= 0 || coord < (-1) * size) {
            coord -= koef * size;
        }
        else {
            coord += size;
        }
        return coord;
    }

    int num_byte(int ind) {
        return (ind % 8 != 0 || ind == 0) ? ind / 8 + 1: ind / 8;
    }

    int num_in_byte(int column) {
        if (column < 8) {
            return column;
        }
        return column % 8 == 0? 0: column % 8;
    }

    bool check_bit(unsigned char byte, int pos) {
        return static_cast<bool>((1 << (7 - pos)) & byte);
    }

    void set_true(unsigned char& byte, int pos) {
        byte |= (1 << (7 - pos));
    }

    void set_false(unsigned char& chank, int pos) {
        chank &= ~(1 << (7 - pos));
    }
}

Field::Field() {
    field_.resize(size_ * num_byte(size_), 0);
}

Field::Field(std::vector<Point> coords) {
    field_.resize(size_ * num_byte(size_), 0);
    
    for (auto& i: coords) {
        i.x = normalize(i.x, size_);
        i.y = normalize(i.y, size_);
        int num_chank = i.x * num_byte(size_) + num_byte(i.y);
        int pos = num_in_byte(i.y);
        num_chank += i.y % 8 == 0 && i.y != 0? 0: -1;

        if (check_bit(field_[num_chank], pos)) {
            throw std::invalid_argument("Some coordinates repeated;");
        }
        else {
            set_true(field_[num_chank], pos);
        }
    }
}

bool Field::get_state(int row, int column) const{
    row = normalize(row, size_);
    column = normalize(column, size_);
    int num_chank = row * num_byte(size_) + num_byte(column);
    int pos = num_in_byte(column);
    num_chank += column % 8 == 0 && column != 0? 0: -1;
    return check_bit(field_[num_chank], pos);
}

void Field::set_state(int row, int column, bool value) {
    row = normalize(row, size_);
    column = normalize(column, size_);
    int num_chank = row * num_byte(size_) + num_byte(column);
    int pos = num_in_byte(column);
    num_chank += column % 8 == 0 && column != 0? 0: -1;

    if (value) {
        set_true(field_[num_chank], pos);
    }
    else {
        set_false(field_[num_chank], pos);
    }
}

Field::Field(const Field& other) {
    field_ = other.field_;
}

Field& Field::operator=(const Field& other) {
    field_ = other.field_;
    return *this;
}

int Field::get_size() const {
    return size_;
}