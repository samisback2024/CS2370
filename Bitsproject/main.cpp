#include <iostream>
#include "bits.h"

int main() {
    Bits b1(5);  // Initialize with the number 5 (binary 101)
    Bits b2;

    std::cout << "Initial value of b1: " << b1 << std::endl;
    std::cout << "Initial value of b2: " << b2 << std::endl;

    // Test set and reset
    b1.set(1);
    std::cout << "After setting bit 1 of b1: " << b1 << std::endl;

    b1.reset(0);
    std::cout << "After resetting bit 0 of b1: " << b1 << std::endl;

    // Test toggle
    b1.toggle(2);
    std::cout << "After toggling bit 2 of b1: " << b1 << std::endl;

    // Test shift
    b1.shift(2);
    std::cout << "After shifting b1 by 2 to the right: " << b1 << std::endl;

    // Test rotate
    b1.rotate(2);
    std::cout << "After rotating b1 by 2 to the right: " << b1 << std::endl;

    // Test ones and zeroes count
    std::cout << "Number of ones in b1: " << b1.ones() << std::endl;
    std::cout << "Number of zeroes in b1: " << b1.zeroes() << std::endl;

    // Test assign
    b2.assign(10);  // Assigning b2 a value of 10 (binary 1010)
    std::cout << "Value of b2 after assignment: " << b2 << std::endl;
        
    // Test equality operator
    if (b1 == b2) {
        std::cout << "b1 and b2 are equal." << std::endl;
    }
    else {
        std::cout << "b1 and b2 are not equal." << std::endl;
    }

    return 0;
}
