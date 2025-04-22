#include "Vector.h"
#include <iostream>
#include <stdexcept>
#include "test.h"

int main() {
    Vector v;

    // Test push_back and size
    v.push_back(10);
    test_(v.size() == 1);

    v.push_back(20);
    test_(v.size() == 2);

    // Test front and back
    test_(v.front() == 10);
    test_(v.back() == 20);

    // Test access via operator[]
    test_(v[0] == 10);
    test_(v[1] == 20);

    // Test pop_back
    v.pop_back();
    test_(v.size() == 1);
    test_(v.back() == 10);

    // Test insert
    v.insert(1, 30);
    test_(v.size() == 2);
    test_(v[1] == 30);

    // Test erase
    v.erase(0);
    test_(v.size() == 1);
    test_(v.front() == 30);

    // Test clear
    v.clear();
    test_(v.size() == 0);
    test_(v.empty());

    // Test exception handling
    throw_(v.at(0), std::out_of_range); // Should throw out_of_range exception
    throw_(v.at(0), std::out_of_range); // Expect std::out_of_range to be thrown

    report_();  // Show test results

    return 0;
}

