#ifndef SHORT_GAME_H
#define SHORT_GAME_H

#include <set>
#include <optional>
#include <unordered_map>

#include "DyadicRational.h"
#include "ShortGameUtil.h"
#include "ShortGameDatabase.h"
#include "../Flavours/SuperposedGameState/GameState/Position/Position.h"

enum class OutcomeClass {
    L,
    R,
    N,
    P,
};

struct ShortGameCache {
    std::unordered_map<ShortGameId, std::partial_ordering> comparison = {};
    std::optional<ShortGameId> negativeId;
    std::unordered_map<ShortGameId, ShortGameId> disjunctiveSum = {};
    std::optional<std::string> displayString;
    std::optional<OutcomeClass> outcomeClass, leftStartOutcomeClass, rightStartOutcomeClass;
    std::optional<size_t> birthday;
    std::optional<ShortGameId> canonicalFormId;
    std::optional<bool> isInteger;
    std::optional<bool> isNumber;
    std::optional<DyadicRational> numberValue;
};

// Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
class ShortGame {
public:
    // This constructor should only be used from within ShortGameDatabase.
    // Otherwise, ShortGameDatabase.getOrInsert should be used.
    ShortGame(std::set<ShortGameId> leftOptions, std::set<ShortGameId> rightOptions, ShortGameId id);
    
    // Checks whether two games are isomorphic.
    bool operator==(const ShortGame& other) const {
        // Isomorphic games are assigned the same id
        return id == other.id;
    }
    // Determines the partial ordering between this game and another game.
    std::partial_ordering operator<=>(const ShortGame& other) const;
    // Constructs the negative of this game.
    ShortGame& operator-() const;
    // Constructs the disjunctive sum of this game and another game.
    ShortGame& operator+(const ShortGame& other) const;
    // Constructs the difference game of this game and another game.
    ShortGame& operator-(const ShortGame& other) const;

    const std::set<ShortGameId>& getLeftOptions() const { return leftOptions; }
    const std::set<ShortGameId>& getRightOptions() const { return rightOptions; }
    ShortGameId getId() const { return id; }

    // Recursively constructs a display string representing this short game as a pair of sets of Left and Right options
    std::string determineDisplayString() const;
    // Determines the outcome class of this game.
    OutcomeClass determineOutcomeClass() const;
    // Determines the birthday of this game.
    size_t determineBirthday() const;
    // Determines whether this game is in canonical form.
    bool isInCanonicalForm() const;
    // Determines the canonical form of this game.
    ShortGame& determineCanonicalForm() const;
    // Determines whether this game is in canonical form and a number.
    bool isCanonicalNumber() const;
    // Determines the number value of this game.
    std::optional<DyadicRational> determineNumberValue() const;

    void setCache(const ShortGameCache& cache) const { this->cache = cache; }

private:
    OutcomeClass determineOutcomeClass(Player turn) const;

    const std::set<ShortGameId> leftOptions, rightOptions;
    const ShortGameId id;

    mutable ShortGameCache cache;

    bool isCanonicalInteger() const;
};

#endif // SHORT_GAME_H
