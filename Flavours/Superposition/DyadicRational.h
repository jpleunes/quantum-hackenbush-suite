#ifndef DYADIC_RATIONAL_H
#define DYADIC_RATIONAL_H

#include <optional>

class DyadicRational {
public:
    long long numerator;
    long long denominator;
    DyadicRational() = delete;
    DyadicRational(long long numerator, long long denominator);
    explicit DyadicRational(long long integer);
    DyadicRational(const DyadicRational& other);
    DyadicRational& operator=(const DyadicRational& other);

    bool operator==(const DyadicRational& other) const;
    std::strong_ordering operator<=>(const DyadicRational& other) const;
};

std::optional<DyadicRational> getSimplestNumber(const DyadicRational& left, const DyadicRational& right);

std::ostream& operator<<(std::ostream& os, const DyadicRational& number);

#endif // DYADIC_RATIONAL_H
