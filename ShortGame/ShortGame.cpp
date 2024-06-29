#include <iostream>
#include <algorithm>

#include "ShortGame.h"
#include "ShortGameDatabase.h"

ShortGame::ShortGame(std::set<ShortGameId> leftOptions, std::set<ShortGameId> rightOptions, ShortGameId id) : leftOptions(leftOptions), rightOptions(rightOptions), id(id) {
    if (id == ShortGameDatabase::zeroId) {
        // There is a lot that we already know about the zero game
        cache.negativeId = ShortGameDatabase::zeroId;
        cache.outcomeClass = OutcomeClass::P;
        cache.birthday = 0;
        cache.canonicalFormId = ShortGameDatabase::zeroId;
        cache.isInteger = true;
        cache.isNumber = true;
        cache.numberValue = DyadicRational(0);
    }
}

// Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
std::partial_ordering ShortGame::operator<=> (const ShortGame& other) const {
    const ShortGame& left = (cache.canonicalFormId.has_value()) ? ShortGameDatabase::getInstance().getGame(cache.canonicalFormId.value()) : *this;
    const ShortGame& right = (other.cache.canonicalFormId.has_value()) ? ShortGameDatabase::getInstance().getGame(other.cache.canonicalFormId.value()) : other;

    if (left.cache.comparison.contains(right.getId())) {
        return left.cache.comparison.at(right.getId());
    }
    if (left.cache.isNumber.has_value() && left.cache.isNumber.value() && right.cache.isNumber.has_value() && right.cache.isNumber.value()) {
        return left.determineNumberValue().value() <=> right.determineNumberValue().value();
    }

    const ShortGame& difference = left - right;
    std::partial_ordering result = std::partial_ordering::unordered;
    switch (difference.determineOutcomeClass()) {
        case OutcomeClass::L:
            result = std::partial_ordering::greater;
            break;
        case OutcomeClass::R:
            result = std::partial_ordering::less;
            break;
        case OutcomeClass::N:
            result = std::partial_ordering::unordered;
            break;
        case OutcomeClass::P:
            result = std::partial_ordering::equivalent;
            break;
        default: throw(std::domain_error("Unknown outcome class case."));
    }

    cache.comparison.insert({ other.getId(), result });
    cache.comparison.insert({ right.getId(), result });
    left.cache.comparison.insert({ other.getId(), result });
    left.cache.comparison.insert({ right.getId(), result });
    std::partial_ordering reversedResult = result;
    if (result == std::partial_ordering::less) reversedResult = std::partial_ordering::greater;
    else if (result == std::partial_ordering::greater) reversedResult = std::partial_ordering::less;
    other.cache.comparison.insert({ id, reversedResult });
    other.cache.comparison.insert({ left.getId(), reversedResult });
    right.cache.comparison.insert({ id, reversedResult });
    right.cache.comparison.insert({ left.getId(), reversedResult });

    return result;
}

// Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
ShortGame& ShortGame::operator-() const {
    if (id == ShortGameDatabase::getInstance().zeroId) return ShortGameDatabase::getInstance().getZeroGame();
    if (cache.negativeId.has_value()) return ShortGameDatabase::getInstance().getGame(cache.negativeId.value());
    
    std::set<ShortGameId> resultLeftOptions, resultRightOptions;
    for (ShortGameId leftOption : leftOptions) resultRightOptions.insert((-(ShortGameDatabase::getInstance().getGame(leftOption))).getId());
    for (ShortGameId rightOption : rightOptions) resultLeftOptions.insert((-(ShortGameDatabase::getInstance().getGame(rightOption))).getId());
    ShortGame& result = ShortGameDatabase::getInstance().getOrInsert(resultLeftOptions, resultRightOptions);

    cache.negativeId = result.getId();

    return result;
}

// Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
ShortGame& ShortGame::operator+(const ShortGame& other) const {
    if (id == ShortGameDatabase::zeroId) return ShortGameDatabase::getInstance().getGame(other.getId());
    if (other.getId() == ShortGameDatabase::zeroId) return ShortGameDatabase::getInstance().getGame(id);

    if (id == other.cache.negativeId || other.getId() == cache.negativeId) return ShortGameDatabase::getInstance().getZeroGame();
    if (cache.disjunctiveSum.contains(other.getId())) return ShortGameDatabase::getInstance().getGame(cache.disjunctiveSum.at(other.getId()));
    if (other.cache.disjunctiveSum.contains(id)) return ShortGameDatabase::getInstance().getGame(other.cache.disjunctiveSum.at(id));
    if (cache.isNumber.has_value() && cache.isNumber.value() && other.cache.isNumber.has_value() && other.cache.isNumber.value()) {
        return ShortGameDatabase::getInstance().getOrInsert(determineNumberValue().value() + other.determineNumberValue().value());
    }

    std::set<ShortGameId> resultLeftOptions, resultRightOptions;
    for (ShortGameId leftOption : leftOptions) resultLeftOptions.insert((ShortGameDatabase::getInstance().getGame(leftOption) + other).getId());
    for (ShortGameId leftOption : other.leftOptions) resultLeftOptions.insert((*this + ShortGameDatabase::getInstance().getGame(leftOption)).getId());
    for (ShortGameId rightOption : rightOptions) resultRightOptions.insert((ShortGameDatabase::getInstance().getGame(rightOption) + other).getId());
    for (ShortGameId rightOption : other.rightOptions) resultRightOptions.insert((*this + ShortGameDatabase::getInstance().getGame(rightOption)).getId());
    ShortGame& result = ShortGameDatabase::getInstance().getOrInsert(resultLeftOptions, resultRightOptions);
    
    cache.disjunctiveSum.insert({ other.getId(), result.getId() });
    other.cache.disjunctiveSum.insert({ id, result.getId() });

    return result;
}

ShortGame& ShortGame::operator-(const ShortGame& other) const {
    return *this + (-other);
}

std::string ShortGame::determineDisplayString() const {
    if (cache.displayString.has_value()) return cache.displayString.value();
    std::ostringstream result;
    std::optional<DyadicRational> numberValue = determineNumberValue();
    if (numberValue.has_value()) {
        result << numberValue.value();
        cache.displayString = result.str();
        return cache.displayString.value();
    }
    if (leftOptions.empty() && rightOptions.empty()) {
        cache.displayString = "0";
        return "0";
    }
    result << "{";
    for (ShortGameId leftOption : leftOptions) {
        result << ShortGameDatabase::getInstance().getGame(leftOption).determineDisplayString() << ",";
    }
    result << "|";
    for (ShortGameId rightOption : rightOptions) {
        result << ShortGameDatabase::getInstance().getGame(rightOption).determineDisplayString() << ",";
    }
    result << "}";
    cache.displayString = result.str();
    return cache.displayString.value();
}

OutcomeClass ShortGame::determineOutcomeClass(Player turn) const {
    if (turn == Player::LEFT && cache.leftStartOutcomeClass.has_value()) return cache.leftStartOutcomeClass.value();
    else if (turn == Player::RIGHT && cache.rightStartOutcomeClass.has_value()) return cache.rightStartOutcomeClass.value();

    switch (turn) {
        case Player::LEFT:
            for (ShortGameId option : leftOptions) {
                OutcomeClass outcome = ShortGameDatabase::getInstance().getGame(option).determineOutcomeClass(Player::RIGHT);
                if (outcome == OutcomeClass::L || outcome == OutcomeClass::P) {
                    cache.leftStartOutcomeClass = OutcomeClass::L;
                    return OutcomeClass::L;
                }
            }
            cache.leftStartOutcomeClass = OutcomeClass::R;
            return OutcomeClass::R;
        case Player::RIGHT:
            for (ShortGameId option : rightOptions) {
                OutcomeClass outcome = ShortGameDatabase::getInstance().getGame(option).determineOutcomeClass(Player::LEFT);
                if (outcome == OutcomeClass::R || outcome == OutcomeClass::P) {
                    cache.rightStartOutcomeClass = OutcomeClass::R;
                    return OutcomeClass::R;
                }
            }
            cache.rightStartOutcomeClass = OutcomeClass::L;
            return OutcomeClass::L;
        default:
            throw(std::domain_error("Unknown player case."));
    }
}

// Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
OutcomeClass ShortGame::determineOutcomeClass() const {
    if (cache.outcomeClass.has_value()) return cache.outcomeClass.value();

    OutcomeClass leftStartOutcome = determineOutcomeClass(Player::LEFT);
    OutcomeClass rightStartOutcome = determineOutcomeClass(Player::RIGHT);
    bool leftHasWinningMove = leftStartOutcome == OutcomeClass::L || leftStartOutcome == OutcomeClass::P;
    bool rightHasWinningMove = rightStartOutcome == OutcomeClass::R || rightStartOutcome == OutcomeClass::P;
    if (leftHasWinningMove && rightHasWinningMove) {
        cache.outcomeClass = OutcomeClass::N;
        return OutcomeClass::N;
    }
    if (leftHasWinningMove) {
        cache.outcomeClass = OutcomeClass::L;
        return OutcomeClass::L;
    }
    else if (rightHasWinningMove) {
        cache.outcomeClass = OutcomeClass::R;
        return OutcomeClass::R;
    }
    else {
        cache.outcomeClass = OutcomeClass::P;
        return OutcomeClass::P;
    }
}

size_t ShortGame::determineBirthday() const {
    if (cache.birthday.has_value()) return cache.birthday.value();

    size_t result = 0;
    if (leftOptions.empty() && rightOptions.empty()) {
        cache.birthday = 0;

        return 0;
    }
    for (ShortGameId leftOption : leftOptions) result = std::max(result, ShortGameDatabase::getInstance().getGame(leftOption).determineBirthday());
    for (ShortGameId rightOption : rightOptions) result = std::max(result, ShortGameDatabase::getInstance().getGame(rightOption).determineBirthday());
    result += 1;

    cache.birthday = result;

    return result;
}

// Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
bool ShortGame::isInCanonicalForm() const {
    if (!cache.canonicalFormId.has_value()) determineCanonicalForm();
    return id == cache.canonicalFormId.value();
}

// Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
ShortGame& ShortGame::determineCanonicalForm() const {
    if (cache.canonicalFormId.has_value()) return ShortGameDatabase::getInstance().getGame(cache.canonicalFormId.value());
    
    if (determineOutcomeClass() == OutcomeClass::P) {
        cache.canonicalFormId = ShortGameDatabase::zeroId;
        return ShortGameDatabase::getInstance().getZeroGame();
    }

    // Convert all options into canonical form
    std::set<ShortGameId> canonicalLeftOptions, canonicalRightOptions;
    for (ShortGameId leftOption : leftOptions) {
        canonicalLeftOptions.insert(ShortGameDatabase::getInstance().getGame(leftOption).determineCanonicalForm().getId());
    }
    for (ShortGameId rightOption : rightOptions) {
        canonicalRightOptions.insert(ShortGameDatabase::getInstance().getGame(rightOption).determineCanonicalForm().getId());
    }

    // Remove dominated options
    std::set<ShortGameId> undominatedLeftOptions, undominatedRightOptions;
    for (ShortGameId leftOption : canonicalLeftOptions) {
        // For every Left option, we only want to keep it if it is not smaller than any other Left
        // option, and it is not equal to another Left option that we are keeping
        bool removeOption = false;
        for (ShortGameId otherLeftOption : canonicalLeftOptions) {
            if (leftOption != otherLeftOption && ShortGameDatabase::getInstance().getGame(leftOption) < ShortGameDatabase::getInstance().getGame(otherLeftOption)) {
                removeOption = true;
                break;
            }
        }
        if (removeOption) continue;
        for (ShortGameId undominatedLeftOption : undominatedLeftOptions) {
            if (ShortGameDatabase::getInstance().getGame(leftOption) <=> ShortGameDatabase::getInstance().getGame(undominatedLeftOption) == std::partial_ordering::equivalent) {
                removeOption = true;
                break;
            }
        }
        if (removeOption) continue;
        undominatedLeftOptions.insert(leftOption);
    }
    for (ShortGameId rightOption : canonicalRightOptions) {
        // For every Right option, we only want to keep it if it is not larger than any other Right
        // option, and it is not equal to another Right option that we are keeping
        bool removeOption = false;
        for (ShortGameId otherRightOption : canonicalRightOptions) {
            if (rightOption != otherRightOption && ShortGameDatabase::getInstance().getGame(rightOption) > ShortGameDatabase::getInstance().getGame(otherRightOption)) {
                removeOption = true;
                break;
            }
        }
        if (removeOption) continue;
        for (ShortGameId undominatedRightOption : undominatedRightOptions) {
            if (ShortGameDatabase::getInstance().getGame(rightOption) <=> ShortGameDatabase::getInstance().getGame(undominatedRightOption) == std::partial_ordering::equivalent) {
                removeOption = true;
                break;
            }
        }
        if (removeOption) continue;
        undominatedRightOptions.insert(rightOption);
    }

    // Use reversibility to replace reversible options by their corresponding replacement sets
    std::set<ShortGameId> resultLeftOptions, resultRightOptions;
    bool reversibilityApplied = false;
    for (ShortGameId leftOption : undominatedLeftOptions) {
        ShortGame& leftGame = ShortGameDatabase::getInstance().getGame(leftOption);
        bool reversibleOption = false;
        for (ShortGameId leftRightOption : leftGame.rightOptions) {
            ShortGame& leftRightGame = ShortGameDatabase::getInstance().getGame(leftRightOption);
            if (leftRightGame <= *this) {
                reversibilityApplied = true;
                reversibleOption = true;
                resultLeftOptions.insert(leftRightGame.leftOptions.begin(), leftRightGame.leftOptions.end());
                break;
            }
        }
        if (!reversibleOption) resultLeftOptions.insert(leftOption);
    }
    for (ShortGameId rightOption : undominatedRightOptions) {
        ShortGame& rightGame = ShortGameDatabase::getInstance().getGame(rightOption);
        bool reversibleOption = false;
        for (ShortGameId rightLeftOption : rightGame.leftOptions) {
            ShortGame& rightLeftGame = ShortGameDatabase::getInstance().getGame(rightLeftOption);
            if (rightLeftGame >= *this) {
                reversibilityApplied = true;
                reversibleOption = true;
                resultRightOptions.insert(rightLeftGame.rightOptions.begin(), rightLeftGame.rightOptions.end());
                break;
            }
        }
        if (!reversibleOption) resultRightOptions.insert(rightOption);
    }

    // If we were able to apply reversibility, then we need to recurse back to see if any dominated
    // options may be removed
    if (reversibilityApplied) {
        ShortGame& result = ShortGameDatabase::getInstance().getOrInsert(resultLeftOptions, resultRightOptions).determineCanonicalForm();
        
        cache.canonicalFormId = result.getId();

        return result;
    }

    // Check if the simplest number theorem can be applied
    // TODO: is it more efficient to do this before applying reversibility?
    if (resultLeftOptions.size() == 1 && resultRightOptions.size() == 1 && ShortGameDatabase::getInstance().getGame(*resultLeftOptions.begin()).isCanonicalNumber() && ShortGameDatabase::getInstance().getGame(*resultRightOptions.begin()).isCanonicalNumber()) {
        DyadicRational leftNumber = ShortGameDatabase::getInstance().getGame(*resultLeftOptions.begin()).determineNumberValue().value();
        DyadicRational rightNumber = ShortGameDatabase::getInstance().getGame(*resultRightOptions.begin()).determineNumberValue().value();
        std::optional<DyadicRational> number = determineCanonicalNumberValue(leftNumber, rightNumber);
        if (number.has_value()) {
            ShortGame& result = ShortGameDatabase::getInstance().getOrInsert(number.value());

            cache.canonicalFormId = result.getId();
            result.cache.canonicalFormId = result.getId();

            return result;
        }
    }

    ShortGame& result = ShortGameDatabase::getInstance().getOrInsert(resultLeftOptions, resultRightOptions);

    cache.canonicalFormId = result.getId();
    result.cache.canonicalFormId = result.getId();

    return result;
}

// Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
bool ShortGame::isCanonicalInteger() const {
    if (cache.isInteger.has_value()) return cache.isInteger.value();

    if (id == ShortGameDatabase::zeroId) return true;
    bool result = false;
    if (leftOptions.size() == 1 && rightOptions.empty()) {
        result = ShortGameDatabase::getInstance().getGame(*leftOptions.begin()).isCanonicalInteger();
    }
    else if (leftOptions.empty() && rightOptions.size() == 1) {
        result = ShortGameDatabase::getInstance().getGame(*rightOptions.begin()).isCanonicalInteger();
    }

    cache.isInteger = result;

    return result;
}

// Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
bool ShortGame::isCanonicalNumber() const {
    if (cache.isNumber.has_value()) return cache.isNumber.value();

    if (isCanonicalInteger()) {
        cache.isNumber = true;
        
        return true;
    }
    if (!isInCanonicalForm()) {
        bool result = determineCanonicalForm().isCanonicalNumber();

        cache.isNumber = result;

        return result;
    }
    // This game must now be in canonical form, and not an integer, so if it is a number it must 
    // have exactly one Left option and one Right option for the simplest number theorem to be 
    // applicable.
    if (leftOptions.size() != 1 || rightOptions.size() != 1) {
        cache.isNumber = false;
        return false;
    }
    ShortGame& left = ShortGameDatabase::getInstance().getGame(*leftOptions.begin());
    ShortGame& right = ShortGameDatabase::getInstance().getGame(*rightOptions.begin());
    if (!left.isCanonicalNumber() || !right.isCanonicalNumber()) {
        cache.isNumber = false;
        return false;
    }
    bool result = left < right;

    cache.isNumber = result;

    return result;
}

// Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
std::optional<DyadicRational> ShortGame::determineNumberValue() const {
    if (cache.numberValue.has_value()) return cache.numberValue.value();

    if (!isCanonicalNumber()) {
        return {};
    }
    if (!isInCanonicalForm()) {
        std::optional<DyadicRational> result = determineCanonicalForm().determineNumberValue();

        cache.numberValue = result;

        return result;
    }
    if (isCanonicalInteger()) {
        int resultInt;
        if (id == ShortGameDatabase::zeroId) {
            resultInt = 0;
        }
        else if (leftOptions.empty()) {
            resultInt = -determineBirthday();
        }
        else {
            resultInt = determineBirthday();
        }

        cache.numberValue = DyadicRational(resultInt);

        return DyadicRational(resultInt);
    }
    DyadicRational leftNumber = ShortGameDatabase::getInstance().getGame(*leftOptions.begin()).determineNumberValue().value();
    DyadicRational rightNumber = ShortGameDatabase::getInstance().getGame(*rightOptions.begin()).determineNumberValue().value();
    DyadicRational result = determineCanonicalNumberValue(leftNumber, rightNumber).value();

    cache.numberValue = result;

    return result;
}
