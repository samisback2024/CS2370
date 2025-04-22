#ifndef VECTOR_H
#define VECTOR_H
#include <stdexcept>
#include <cstddef>

template <typename T>
class Vector {
private:
    static const size_t CHUNK = 10; // Default initial capacity
    size_t capacity;                // Total allocated space
    size_t n_elems;                 // Number of elements
    T* data_ptr;                    // Pointer to dynamic array

    // Grow function to increase capacity
    void grow() {
        capacity += CHUNK;
        T* new_data = new T[capacity];
        for (size_t i = 0; i < n_elems; ++i) {
            new_data[i] = data_ptr[i];
        }
        delete[] data_ptr;
        data_ptr = new_data;
    }

public:
    // Constructor
    Vector() : capacity(CHUNK), n_elems(0), data_ptr(new T[capacity]) {}

    // Copy constructor
    Vector(const Vector& v) : capacity(v.capacity), n_elems(v.n_elems), data_ptr(new T[capacity]) {
        for (size_t i = 0; i < n_elems; ++i) {
            data_ptr[i] = v.data_ptr[i];
        }
    }

    // Copy assignment operator
    Vector& operator=(const Vector& v) {
        if (this != &v) {
            delete[] data_ptr;
            capacity = v.capacity;
            n_elems = v.n_elems;
            data_ptr = new T[capacity];
            for (size_t i = 0; i < n_elems; ++i) {
                data_ptr[i] = v.data_ptr[i];
            }
        }
        return *this;
    }

    // Destructor
    ~Vector() {
        delete[] data_ptr;
    }

    // Accessors
    T front() const {
        if (n_elems == 0) throw std::range_error("Vector is empty");
        return data_ptr[0];
    }

    T back() const {
        if (n_elems == 0) throw std::range_error("Vector is empty");
        return data_ptr[n_elems - 1];
    }

    T at(size_t pos) const {
        if (pos >= n_elems) throw std::range_error("Index out of range");
        return data_ptr[pos];
    }

    size_t size() const {
        return n_elems;
    }

    bool empty() const {
        return n_elems == 0;
    }

    // Mutators
    T& operator[](size_t pos) {
        return data_ptr[pos];
    }

    void push_back(const T& item) {
        if (n_elems == capacity) grow();
        data_ptr[n_elems++] = item;
    }

    void pop_back() {
        if (n_elems == 0) throw std::range_error("Vector is empty");
        --n_elems;
    }

    void erase(size_t pos) {
        if (pos >= n_elems) throw std::range_error("Index out of range");
        for (size_t i = pos; i < n_elems - 1; ++i) {
            data_ptr[i] = data_ptr[i + 1];
        }
        --n_elems;
    }

    void insert(size_t pos, const T& item) {
        if (pos > n_elems) throw std::range_error("Index out of range");
        if (n_elems == capacity) grow();
        for (size_t i = n_elems; i > pos; --i) {
            data_ptr[i] = data_ptr[i - 1];
        }
        data_ptr[pos] = item;
        ++n_elems;
    }

    void clear() {
        n_elems = 0;
    }

    // Iterators
    T* begin() {
        return data_ptr;
    }

    T* end() {
        return data_ptr + n_elems;
    }

    // Comparators
    bool operator==(const Vector& v) const {
        if (n_elems != v.n_elems) return false;
        for (size_t i = 0; i < n_elems; ++i) {
            if (data_ptr[i] != v.data_ptr[i]) return false;
        }
        return true;
    }

    bool operator!=(const Vector& v) const {
        return !(*this == v);
    }
};

#endif
