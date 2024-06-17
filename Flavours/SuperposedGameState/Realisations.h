#ifndef REALISATIONS_H
#define REALISATIONS_H

#include "RealisationsUtil.h"
#include "SuperposedGameState.h"
#include "../../Util/HashUtil.h"

#include <vector>
#include <memory>
#include <set>

struct AdjacencyMatrixRealisations {
    AdjacencyMatrixRealisations(std::set<GameStateId> gameStateIds, size_t depth) : gameStateIds(gameStateIds), depth(depth) {
    }

    bool operator==(const AdjacencyMatrixRealisations& other) const {
        return gameStateIds == other.gameStateIds;
    }

    const std::set<GameStateId> gameStateIds;
    const size_t depth;
};

namespace std {
    template<>
    struct hash<AdjacencyMatrixRealisations> {
        size_t operator()(const AdjacencyMatrixRealisations adjacencyMatrixRealisations) const {
            std::vector<uint32_t> intVec;
            for (GameStateId realisationId : adjacencyMatrixRealisations.gameStateIds) intVec.push_back((uint32_t) realisationId);
            return std::hash<std::vector<uint32_t>>()(intVec);
        }
    };
}

struct ShortHollyhocksRealisations {
    ShortHollyhocksRealisations(std::set<GameStateId> gameStateIds, size_t depth) : gameStateIds(gameStateIds), depth(depth) {
    }

    bool operator==(const ShortHollyhocksRealisations& other) const {
        std::multiset<ShortHollyhocksCounts> counts, otherCounts;
        for (GameStateId gameStateId : gameStateIds) counts.insert(((ShortHollyhocksPosition) GameStateDatabase<ShortHollyhocksPosition>::getInstance().getGameState(gameStateId).getPosition()).getCounts());
        for (GameStateId gameStateId : other.gameStateIds) otherCounts.insert(((ShortHollyhocksPosition) GameStateDatabase<ShortHollyhocksPosition>::getInstance().getGameState(gameStateId).getPosition()).getCounts());
        return counts == otherCounts && depth == other.depth;
    }

    const std::set<GameStateId> gameStateIds;
    const size_t depth;
};

namespace std {
    template<>
    struct hash<ShortHollyhocksRealisations> {
        size_t operator()(const ShortHollyhocksRealisations shortHollyhocksRealisations) const {
            std::multiset<ShortHollyhocksCounts> realisationCounts;
            for (GameStateId gameStateId : shortHollyhocksRealisations.gameStateIds) realisationCounts.insert(((ShortHollyhocksPosition) GameStateDatabase<ShortHollyhocksPosition>::getInstance().getGameState(gameStateId).getPosition()).getCounts());

            std::vector<uint32_t> intVec;
            for (ShortHollyhocksCounts counts : realisationCounts) intVec.push_back((uint32_t) std::hash<ShortHollyhocksCounts>()(counts));
            intVec.push_back(shortHollyhocksRealisations.depth);
            return std::hash<std::vector<uint32_t>>()(intVec);
        }

        // TODO: use sorted ShortHollyhocksCounts and depth
    };
}

#endif // REALISATIONS_H
