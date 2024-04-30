#ifndef SHORT_GAME_DATABASE
#define SHORT_GAME_DATABASE

#include <vector>
#include <memory>
#include <unordered_map>
#include <boost/container_hash/hash.hpp>

#include "../Util/HashUtil.h"
#include "ShortGameUtil.h"
#include "DyadicRational.h"

namespace std {
    template<>
    struct hash<std::pair<std::set<ShortGameId>, std::set<ShortGameId>>> {
        size_t operator()(const std::pair<std::set<ShortGameId>, std::set<ShortGameId>>& pair) const {
            std::vector<uint32_t> firstIntVec;
            for (ShortGameId x : pair.first) firstIntVec.push_back((uint32_t) x);
            size_t hash = std::hash<std::vector<uint32_t>>()(firstIntVec);
            std::vector<uint32_t> secondIntVec;
            for (ShortGameId x : pair.second) secondIntVec.push_back((uint32_t) x);
            boost::hash_combine(hash, std::hash<std::vector<uint32_t>>()(secondIntVec));

            return hash;
        }
    };
}

// Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
class ShortGameDatabase {
public:
    static ShortGameDatabase& getInstance() { return instance; }
    ShortGameDatabase(const ShortGameDatabase& other) = delete;

    ShortGame& getOrInsert(const std::set<ShortGameId>& leftOptions, const std::set<ShortGameId>& rightOptions);
    ShortGame& getGame(ShortGameId id) { return *existingGames[id]; }

    ShortGame& getOrInsert(long integer);
    ShortGame& getOrInsert(DyadicRational number);
    ShortGame& getZeroGame() { return getGame(zeroId); }

    const static ShortGameId zeroId = 0;

private:
    ShortGameDatabase();
    std::vector<std::unique_ptr<ShortGame>> existingGames;    
    std::unordered_map<std::pair<std::set<ShortGameId>, std::set<ShortGameId>>, ShortGameId> optionsToId;
    std::unordered_map<DyadicRational, ShortGameId> numberToId;

    static ShortGameDatabase instance;
};

// Prevent recursive dependence
#include "ShortGame.h"

#endif // SHORT_GAME_DATABASE
