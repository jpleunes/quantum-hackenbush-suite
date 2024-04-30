#ifndef DYADIC_RATIONAL_H
#define DYADIC_RATIONAL_H

#include <optional>
#include <climits>

// Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
class DyadicRational {
public:
    long numerator;
    long denominator;
    DyadicRational() = delete;
    DyadicRational(long numerator, long denominator);
    explicit DyadicRational(long integer);
    DyadicRational(const DyadicRational& other);
    DyadicRational& operator=(const DyadicRational& other);

    DyadicRational simplify() const;
    bool operator==(const DyadicRational& other) const;
    DyadicRational operator-() const;
    DyadicRational operator+(const DyadicRational& other) const;
    DyadicRational operator-(const DyadicRational& other) const;
    std::strong_ordering operator<=>(const DyadicRational& other) const;
};

std::optional<DyadicRational> determineSimplestNumber(const DyadicRational& left, const DyadicRational& right);

std::ostream& operator<<(std::ostream& os, const DyadicRational& number);

namespace std {
    template<>
    struct hash<DyadicRational> {
        size_t operator()(const DyadicRational dyadicRational) const {
            unsigned long x = (unsigned long) dyadicRational.numerator;
            unsigned long y = (unsigned long) dyadicRational.denominator;

            // Compress x and y to only half the number of bits
            const unsigned long mask = (1l << (sizeof(long) * CHAR_BIT / 2)) - 1;
            x &= mask;
            y &= mask;

            // Concatenate them
            return (x << (sizeof(long) * CHAR_BIT / 2)) | y;
        }
    };
}

#endif // DYADIC_RATIONAL_H
