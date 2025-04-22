#ifndef BITS_H
#define BITS_H

#include <iostream>
#include <bitset>
#include <cassert>

class Bits {
    using IType = unsigned long long; // We only have to change the integer type here, if desired
    enum { NBITS = sizeof(IType) * 8 };
    IType bits = 0;

public:
    Bits() = default;
    Bits(IType n) {
        bits = n;
    }

    static int size() {
        return NBITS;
    }

    bool at(int pos) const {  // Returns (tests) the bit at bit-position pos
        assert(0 <= pos && pos < NBITS);
        return bits & (IType(1) << pos);
    }

    void set(int pos) {  // Sets the bit at position pos
        assert(0 <= pos && pos < NBITS);
        bits |= (IType(1) << pos);
    }

    void set() {  // Sets all bits
        bits = ~IType(0);  // Set all bits to 1
    }

    void reset(int pos) {  // Resets (makes zero) the bit at position pos
        assert(0 <= pos && pos < NBITS);
        bits &= ~(IType(1) << pos);
    }

    void reset() {  // Resets all bits
        bits = 0;  // Set all bits to 0
    }

    void assign(int pos, bool val) {  // Sets or resets the bit at position pos depending on val
        assert(0 <= pos && pos < NBITS);
        if (val)
            set(pos);
        else
            reset(pos);
    }

    void assign(IType n) {  // Replaces the underlying integer with n
        bits = n;
    }

    void toggle(int pos) {  // Flips the bit at position pos
        assert(0 <= pos && pos < NBITS);
        bits ^= (IType(1) << pos);
    }

    void toggle() {  // Flips all bits
        bits ^= ~IType(0);
    }

    void shift(int n) {  // If n > 0, shifts bits right n places; if n < 0, shifts left
        if (n > 0)
            bits >>= n;
        else if (n < 0)
            bits <<= -n;
    }

    void rotate(int n) {  // If n > 0, rotates right n places; if n < 0, rotates left
        if (n > 0) {
            n %= NBITS;  // Ensure rotation is within bounds
            bits = (bits >> n) | (bits << (NBITS - n));
        }
        else if (n < 0) {
            n = -n % NBITS;  // Ensure rotation is within bounds
            bits = (bits << n) | (bits >> (NBITS - n));
        }
    }

    int ones() const {  // Returns how many bits are set in the underlying integer
        return std::bitset<NBITS>(bits).count();
    }

    int zeroes() const {  // Returns how many bits are reset in the underlying integer
        return NBITS - ones();
    }

    IType to_int() const {
        return bits;
    }

    friend bool operator==(const Bits& b1, const Bits& b2) {
        return b1.bits == b2.bits;
    }

    friend bool operator!=(const Bits& b1, const Bits& b2) {
        return b1.bits != b2.bits;
    }

    friend std::ostream& operator<<(std::ostream& os, const Bits& b) {
        return os << std::bitset<NBITS>(b.bits);  // Be sure to #include <bitset>
    }
};

#endif  // BITS_H
