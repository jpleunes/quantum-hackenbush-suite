#include <bit>
#include <stdexcept>
#include <iostream>

#include "DyadicRational.h"

// Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
DyadicRational::DyadicRational(long numerator, long denominator) : numerator(numerator), denominator(denominator) {
    if (!std::has_single_bit((unsigned long) std::abs(denominator)))
        throw(std::domain_error("Dyadic rational must have a power of 2 as the denominator, but got: " + std::to_string(denominator)));
}

DyadicRational::DyadicRational(long integer) : DyadicRational(integer, 1) {
}

DyadicRational::DyadicRational(const DyadicRational& other) : numerator(other.numerator), denominator(other.denominator) {
}

DyadicRational& DyadicRational::operator=(const DyadicRational& other) = default;

bool DyadicRational::operator==(const DyadicRational& other) const {
    if (denominator == other.denominator) return numerator == other.numerator;
    return numerator * other.denominator == denominator * other.numerator;
}

// Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
DyadicRational DyadicRational::simplify() const {
    long resultNumerator = numerator, resultDenominator = denominator;
    if (resultDenominator < 0) {
        resultNumerator *= -1;
        resultDenominator *= -1;
    }
    if (resultNumerator == 0) resultDenominator = 1;
    while (resultNumerator % 2 == 0 && resultDenominator != 1) {
        resultNumerator /= 2;
        resultDenominator /= 2;
    }
    return DyadicRational(resultNumerator, resultDenominator);
}

DyadicRational DyadicRational::operator-() const {
    return DyadicRational(-numerator, denominator);
}

// Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
DyadicRational DyadicRational::operator+(const DyadicRational& other) const {
    if (denominator == other.denominator) {
        return DyadicRational(numerator + other.numerator, denominator).simplify();
    }
    else if (denominator < other.denominator) {
        return DyadicRational((other.denominator / denominator) * numerator + other.numerator, other.denominator);
    }
    else {
        return DyadicRational((denominator / other.denominator) * other.numerator + numerator, denominator);
    }
}

DyadicRational DyadicRational::operator-(const DyadicRational& other) const {
    return *this + (-other);
}

std::strong_ordering DyadicRational::operator<=>(const DyadicRational& other) const {
    if (denominator == other.denominator) return numerator <=> other.numerator;
    return numerator * other.denominator <=> denominator * other.numerator;
}

/**
 * Uses the simplest number theorem to find the dyadic rational with the smallest power of 2 in 
 * the denominator, that lies strictly between left and right. If left >= right, then this function
 * returns {}, since the simplest number theorem is not applicable in that case.
 * Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
 */
std::optional<DyadicRational> determineSimplestNumber(const DyadicRational& left, const DyadicRational& right) {
    if (left >= right) return {};
    if (left >= DyadicRational(0)) {
        DyadicRational bestGuess(left.numerator / left.denominator + 1);
        while (bestGuess >= right) {
            bestGuess = DyadicRational(bestGuess.numerator * 2 - 1, bestGuess.denominator * 2);
            while (bestGuess <= left) {
                bestGuess = DyadicRational(bestGuess.numerator * 2 + 1, bestGuess.denominator * 2);
            }
        }
        return bestGuess;
    }
    else if (right <= DyadicRational(0)) {
        DyadicRational bestGuess = DyadicRational(right.numerator / right.denominator - 1);
        while (bestGuess <= left) {
            bestGuess = DyadicRational(bestGuess.numerator * 2 + 1, bestGuess.denominator * 2);
            while (bestGuess >= right) {
                bestGuess = DyadicRational(bestGuess.numerator * 2 - 1, bestGuess.denominator * 2);
            }
        }
        return bestGuess;
    }
    else return DyadicRational(0); // left < 0 < right, so we can take 0
}

std::ostream& operator<<(std::ostream& os, const DyadicRational& number) {
    os << number.numerator << "/" << number.denominator;
    return os;
}
