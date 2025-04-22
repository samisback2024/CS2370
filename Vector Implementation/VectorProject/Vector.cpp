#include "Vector.h"
#include <iostream>
#include <stdexcept>

// Constructor
Vector::Vector() : capacity(CHUNK), n_elems(0), data_ptr(new int[capacity]) {}

// Copy constructor
Vector::Vector(const Vector& v) : capacity(v.capacity), n_elems(v.n_elems), data_ptr(new int[capacity]) {
    for (size_t i = 0; i < n_elems; ++i) {
        data_ptr[i] = v.data_ptr[i];
    }
}

// Copy assignment operator
Vector& Vector::operator=(const Vector& v) {
    if (this != &v) {
        delete[] data_ptr;
        capacity = v.capacity;
        n_elems = v.n_elems;
        data_ptr = new int[capacity];
        for (size_t i = 0; i < n_elems; ++i) {
            data_ptr[i] = v.data_ptr[i];
        }
    }
    return *this;
}

// Destructor
Vector::~Vector() {
    delete[] data_ptr;
}

// Grow function
void Vector::grow() {
    capacity += CHUNK;
    int* new_data = new int[capacity];
    for (size_t i = 0; i < n_elems; ++i) {
        new_data[i] = data_ptr[i];
    }
    delete[] data_ptr;
    data_ptr = new_data;
}

// Accessors
int Vector::front() const {
    if (n_elems == 0) throw std::out_of_range("Vector is empty");
    return data_ptr[0];
}

int Vector::back() const {
    if (n_elems == 0) throw std::out_of_range("Vector is empty");
    return data_ptr[n_elems - 1];
}

int Vector::at(size_t pos) const {
    if (pos >= n_elems) throw std::out_of_range("Index out of range");
    return data_ptr[pos];
}

size_t Vector::size() const {
    return n_elems;
}

bool Vector::empty() const {
    return n_elems == 0;
}

// Mutators
int& Vector::operator[](size_t pos) {
    return data_ptr[pos];
}

void Vector::push_back(int item) {
    if (n_elems == capacity) grow();
    data_ptr[n_elems++] = item;
}

void Vector::pop_back() {
    if (n_elems > 0) --n_elems;
}

void Vector::erase(size_t pos) {
    if (pos >= n_elems) throw std::out_of_range("Index out of range");
    for (size_t i = pos; i < n_elems - 1; ++i) {
        data_ptr[i] = data_ptr[i + 1];
    }
    --n_elems;
}

void Vector::insert(size_t pos, int item) {
    if (pos > n_elems) throw std::out_of_range("Index out of range");
    if (n_elems == capacity) grow();
    for (size_t i = n_elems; i > pos; --i) {
        data_ptr[i] = data_ptr[i - 1];
    }
    data_ptr[pos] = item;
    ++n_elems;
}

void Vector::clear() {
    n_elems = 0;
}

// Iterators
int* Vector::begin() {
    return n_elems > 0 ? data_ptr : nullptr;
}

int* Vector::end() {
    return n_elems > 0 ? data_ptr + n_elems : nullptr;
}

// Comparators
bool Vector::operator==(const Vector& v) const {
    if (n_elems != v.n_elems) return false;
    for (size_t i = 0; i < n_elems; ++i) {
        if (data_ptr[i] != v.data_ptr[i]) return false;
    }
    return true;
}

bool Vector::operator!=(const Vector& v) const {
    return !(*this == v);
}
