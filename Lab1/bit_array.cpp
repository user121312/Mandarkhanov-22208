#include "bit_array.h"

namespace {
    using type = BitArray::Type;
    constexpr auto type_size = sizeof(type);

    int num_type(int num_bits) {
        return num_bits % type_size == 0 ? num_bits / type_size : num_bits / type_size + 1;
    }

    void set_true(type &x, int i) {
        x |= (1 << i);
    }

    void set_false(type &x, int i) {
        x &= ~(1 << i); 
    }

    bool check_bit(type x, int i) {
        return (bool((1 << i)  &  x));
    }

    bool check_bit(unsigned long ul, int i) {//перегрузка функции
        return (bool((1 << i)  &  ul));
    }

    int vector_index(int i) {
        return (type_size - 1) - i % type_size;
    }
}

BitArray::BitArray() {}
BitArray::~BitArray() {}

BitArray::BitArray(int num_bits, unsigned long value) { 
    if (num_bits < 0) {
        throw std::invalid_argument("invalid size");
    }
    data_.resize(num_type(num_bits), 0);
    size_ = num_bits;
    if (value == 0) {
        return;
    }
    for (int i = 0; i < size_ && i < type_size * 8; ++i) {
        if (check_bit(value, i)) {
            set_true(data_[i / type_size], vector_index(i));
        }
    }     
}

BitArray::BitArray(const BitArray& other) {
    data_ = other.data_;
    size_ = other.size_;
}

void BitArray::swap(BitArray& other) {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
}

BitArray& BitArray::operator=(const BitArray& other) {
    data_ = other.data_;
    size_ = other.size_;
    return *this;
}

void BitArray::resize(int num_bits, bool value) {
    if (num_bits == size_) {
        return;
    }
    
    if (num_bits > size_) {
        std::vector<type>new_data(num_type(num_bits));

        for (int i = 0; i < size_; ++i) {
            check_bit(data_[i / type_size], vector_index(i)) ? set_true(new_data[i / type_size], vector_index(i)) : set_false(new_data[i / type_size], vector_index(i));
        }

        for (int i = size_; i < num_bits; ++i) {
            if (value) {
                set_true(new_data[i / type_size], vector_index(i));
            }
        }
        data_ = std::move(new_data);
        size_ = num_bits;
        return;
    }
    //(num_bits < size_) - Обрезаем
    for (int i = 0; i < (size_ - num_bits) / type_size; ++i) {
        data_.pop_back();
    }
    size_ = num_bits;
    if (((size_ - num_bits) & type_size) == 0) {
        for (int i = size_; i < (size_ / type_size + 1) * type_size; ++i) { //зануляем биты в оставшихся ячейках последнего чара
            set_false(data_[size_ / type_size],vector_index(i));
        }
    }        
}

std::string BitArray::to_string() const {
    std::string str;
    str.resize(size_);
    for (int i = 0; i < size_; ++i) {
        str[i] = check_bit(data_[i / type_size],vector_index(i)) ? '1' : '0';
    }
    return str;
}

void BitArray::clear() { //удаляет все элементы вертора
    for (int i = 0; i < num_type(size_); ++i) {
        data_.pop_back();
    }
    size_ = 0;
}

void BitArray::push_back(bool bit) {
    if (size_ % type_size != 0) {
        if (bit) {
            set_true(data_[size_ / type_size], vector_index(size_));
        }
        ++size_;
        return;
    }
    data_.push_back(0);
    ++size_;
    if (bit) {
        set_true(data_[size_ / type_size], type_size - 1); 
    }
}

BitArray& BitArray::operator&=(const BitArray& b) {
    if (size_ != b.size_) {
        throw std::invalid_argument("different sizes");
    }
    for (int i = 0; i < size_; ++i) {
        if (check_bit(data_[i / type_size], vector_index(i)) == 0 || check_bit(b.data_[i / type_size], vector_index(i)) == 0) {
            set_false (data_[i / type_size], vector_index(i));
        }
    }
    return *this;
}

BitArray& BitArray::operator|=(const BitArray& b) {
    if (size_ != b.size_) {
        throw std::invalid_argument("different sizes");
    }
    for (int i = 0; i < size_; ++i) {
        if (check_bit(data_[i / type_size], vector_index(i)) == 0 && check_bit(b.data_[i / type_size], vector_index(i)) == 1) {
            set_true (data_[i / type_size], vector_index(i));
        }
    }
    return *this;
}

BitArray& BitArray::operator^=(const BitArray& b) {
    if (size_ != b.size_) {
        throw std::invalid_argument("different sizes");
    }
    for (int i = 0; i < size_; ++i) {
        if (check_bit(data_[i / type_size], vector_index(i)) == check_bit(b.data_[i / type_size], vector_index(i))) {
            set_false(data_[i / type_size], vector_index(i));
        }
        else {
            set_true(data_[i / type_size], vector_index(i));
        }
    }
    return *this;
}

BitArray& BitArray::operator<<=(int n) {
    if (n == 0) {
        return *this;
    }

    if (n < 0) {
        this->operator>>=(-n);
        return *this;
    }

    if (n >= size_) {
        for (int i = 0; i < num_type(size_); ++i){
            data_[i] = 0;
        }
        return *this;
    }

    for (int i = n; i < size_; ++i) {
        if (check_bit(data_[i / type_size], vector_index(i))) {
            set_true(data_[(i - n) / type_size], vector_index(i - n));
        }
        else {
            set_false(data_[(i - n) / type_size], vector_index(i - n));
        }
    }
    for (int i = size_ - n; i < size_; ++i) {
        set_false(data_[i / type_size], vector_index(i));
    }
    return *this;
}

BitArray& BitArray::operator>>=(int n) {
    if (n == 0) {
        return *this;
    }

    if (n < 0) {
        this->operator<<=(-n);
        return *this;
    }

    for (int i = size_ - n; i >= 0; --i) {
        if (check_bit(data_[i / type_size], vector_index(i))) {
            set_true(data_[(i + n) / type_size], vector_index(i + n));
        }
        else {
            set_false(data_[(i + n) / type_size], vector_index(i + n));
        }
    }

    for (int i = size_ - n; i < size_; ++i) {
        set_false(data_[i / type_size], vector_index(i));
    }

    return *this;
}

BitArray BitArray::operator<<(int n) const {
    BitArray new_bit_array(*this);
    return new_bit_array.operator<<=(n);    
}

BitArray BitArray::operator>>(int n) const {
    BitArray new_bit_array(*this);
    return new_bit_array.operator>>=(n); 
}

BitArray& BitArray::set(int n, bool val) {
    if (n < 0 || n >= size_) {
        throw std::out_of_range("invalid_index");
    }
    if (val) {
        set_true(data_[n / type_size], vector_index(n));
        return *this;
    }
    set_false(data_[n / type_size], vector_index(n));
    return *this;
}

BitArray& BitArray::set() {
    for (int i = 0; i < size_ / type_size; ++ i) {
        data_[i] = 255;//? Как вычислить MAX_VALUE(type)
    }
    for (int i = size_ / type_size; i < size_; ++i) {
        set_true(data_[i / type_size], vector_index(i));
    }
    return *this;
}

BitArray& BitArray::reset(int n) {
    if (n < 0 || n >= size_) {
        throw std::out_of_range("invalid_index");
    }
    set_false(data_[n / type_size], vector_index(n));
    return *this;
}

BitArray& BitArray::reset() {
    for (int i = 0; i < num_type(size_); ++ i) {
        data_[i] = 0;
    }
    return *this;
}

bool BitArray::any() const {
    for (int i = 0; i < num_type(size_); ++i) {
        if (data_[i] != 0) {
            return true;
        }
    }
    return false;
}

bool BitArray::none() const{
    for (int i = 0; i < num_type(size_); ++i) {
        if (data_[i] != 0) {
            return false;
        }
    }
    return true;
}

BitArray BitArray::operator~() const {
    BitArray new_bit_array(size_);
    for (int i = 0; i < size_; ++i) {
        if (!check_bit(data_[i / type_size], vector_index(i))) {
            set_true(new_bit_array.data_[i / type_size], vector_index(i));
        }
    }
    return new_bit_array;   
}

int BitArray::count() const {
    int count = 0;
    for (int i = 0; i < size_; ++i) {
        if (check_bit(data_[i / type_size], vector_index(i))) {
            ++count;
        }
    }
    return count;
}

bool BitArray::operator[](int i) const {
    if (i < 0 || i >= size_) {
        throw std::out_of_range("invalid_index");
    }
    return check_bit(data_[i / type_size], vector_index(i));
}

int BitArray::size() const {
    return size_;
}

bool BitArray::empty() const {
    return size_ == 0;
}

bool operator==(const BitArray & a, const BitArray & b) {
    if (a.size() != b.size()) {
        return false;
    }
    return a.to_string() == b.to_string();
}

bool operator!=(const BitArray & a, const BitArray & b) {
    return !(a == b);
}

BitArray operator&(const BitArray& a, const BitArray& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("different sizes");
    }
    BitArray c = a;
    return c.operator&=(b);
}

BitArray operator|(const BitArray& a, const BitArray& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("different sizes");
    }
    BitArray c = a;
    return c.operator|=(b);
}

BitArray operator^(const BitArray& a, const BitArray& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("different sizes");
    }
    BitArray c = a;
    return c.operator^=(b);
}