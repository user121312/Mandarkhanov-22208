#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <exception>

class BitArray {
public:
    using Type = unsigned char;
private:
    std::vector<Type> data_;
    int size_ = 0;
public:
    BitArray();
    ~BitArray();
    BitArray(int num_bits, unsigned long value = 0);
    BitArray(const BitArray& b);

    void swap(BitArray& b);
    BitArray& operator=(const BitArray& b);
    void resize(int num_bits, bool value = false);//Изменяет размер массива. В случае расширения, новые элементы инициализируются значением value.
    void clear();//удаляет все элементы вертора
    void push_back(bool bit);

    BitArray& operator&=(const BitArray& b);
    BitArray& operator|=(const BitArray& b);
    BitArray& operator^=(const BitArray& b);

    BitArray& operator<<=(int n);//Битовый сдвиг с заполнением нулями
    BitArray& operator>>=(int n);//Битовый сдвиг с заполнением нулями
    BitArray operator<<(int n) const;//Битовый сдвиг с заполнением нулями
    BitArray operator>>(int n) const;//Битовый сдвиг с заполнением нулями

    BitArray& set(int n, bool val = true);//Устанавливает бит с индексом n в значение val
    BitArray& set();//Заполняет массив истиной

    BitArray& reset(int n);//Устанавливает бит с индексом n в значение false
    BitArray& reset();//Заполняет массив ложью

    bool any() const;//true, если массив содержит истинный бит
    bool none() const;//true, если все биты массива ложны
    BitArray operator~() const;//Битовая инверсия
    int count() const; //Подсчитывает количество единичных бит
    bool operator[](int i) const;//Возвращает значение бита по индексу i
    int size() const;//Возвращает размер
    bool empty() const;//
    std::string to_string() const;
};

bool operator==(const BitArray & a, const BitArray & b);
bool operator!=(const BitArray & a, const BitArray & b);

BitArray operator&(const BitArray& b1, const BitArray& b2);
BitArray operator|(const BitArray& b1, const BitArray& b2);
BitArray operator^(const BitArray& b1, const BitArray& b2);
