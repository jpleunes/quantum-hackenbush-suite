#include "ShortGameDatabase.h"

ShortGameDatabase ShortGameDatabase::instance = ShortGameDatabase();

// Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
ShortGameDatabase::ShortGameDatabase() {
    existingGames.emplace_back(
        std::make_unique<ShortGame>(std::set<ShortGameId>(), std::set<ShortGameId>(), ShortGameDatabase::zeroId)
    );
    existingGames.reserve(1024);
}

// Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
ShortGame& ShortGameDatabase::getOrInsert(const std::set<ShortGameId>& leftOptions, const std::set<ShortGameId>& rightOptions) {
    if (leftOptions.empty() && rightOptions.empty()) return getZeroGame();
    std::pair<std::set<ShortGameId>, std::set<ShortGameId>> pair = std::make_pair(leftOptions, rightOptions);
    if (optionsToId.contains(pair)) return getGame(optionsToId[pair]);

    ShortGameId id = existingGames.size();
    optionsToId[pair] = id;
    existingGames.push_back(std::make_unique<ShortGame>(leftOptions, rightOptions, id));
    return getGame(id);
}

// Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
ShortGame& ShortGameDatabase::getOrInsert(long integer) {
    if (integer == 0) return getZeroGame();
    if (numberToId.contains(DyadicRational(integer, 1))) return getGame(numberToId[DyadicRational(integer, 1)]);

    if (integer > 0) {
        ShortGame& result = getOrInsert({ getOrInsert(integer - 1).getId() }, {});
        numberToId[DyadicRational(integer, 1)] = result.getId();
        
        result.setCache({
            {},
            {},
            {},
            OutcomeClass::L,
            integer,
            result.getId(),
            true,
            true,
            DyadicRational(integer, 1)
        });

        return result;
    }
    else {
        ShortGame& result = getOrInsert({}, { getOrInsert(integer + 1).getId() });
        numberToId[DyadicRational(integer, 1)] = result.getId();
        
        result.setCache({
            {},
            {},
            {},
            OutcomeClass::R,
            -integer,
            result.getId(),
            true,
            true,
            DyadicRational(integer, 1)
        });

        return result;
    }
    
}

// Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
ShortGame& ShortGameDatabase::getOrInsert(DyadicRational number) {
    const DyadicRational simplifiedNumber = number.simplify();

    if (simplifiedNumber.denominator == 1) 
    return getOrInsert(simplifiedNumber.numerator);
    if (numberToId.contains(simplifiedNumber)) return getGame(numberToId[simplifiedNumber]);

    ShortGame& resultLeftOption = getOrInsert(DyadicRational((simplifiedNumber.numerator - 1) / 2, simplifiedNumber.denominator / 2));
    ShortGame& resultRightOption = getOrInsert(DyadicRational((simplifiedNumber.numerator + 1) / 2, simplifiedNumber.denominator / 2));
    ShortGame& result = getOrInsert({ resultLeftOption.getId() }, { resultRightOption.getId() });
    numberToId[simplifiedNumber] = result.getId();

    result.setCache({
        {},
        {},
        {},
        {},
        {},
        result.getId(),
        false,
        true,
        simplifiedNumber
    });

    return result;
}
