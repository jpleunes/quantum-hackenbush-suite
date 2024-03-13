#ifndef SUPERPOSITION_H
#define SUPERPOSITION_H

#include <vector>
#include <set>

#include "../Position/PositionDatabase.h"

template<typename Realisation>
class Superposition {
public:
    Superposition() = default;
    Superposition(PositionId classicalPositionId);
    Superposition(const Realisation classicalPosition);
    bool empty() const;
    void addRealisationId(PositionId realisationId);
    const std::vector<PositionId> getRealisationIds() const;
    // Gets the pieces which exist in at least one realisation (and can thus be used in a move) 
    // for a given player.
    std::vector<typename Realisation::Piece> getPieces(Player player) const;

    ~Superposition() = default;
    
private:
    std::set<PositionId> realisationIds;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
Superposition<Realisation>::Superposition(PositionId classicalPositionId) : realisationIds({classicalPositionId}) {
}

template<typename Realisation>
Superposition<Realisation>::Superposition(const Realisation classicalPosition) : realisationIds({PositionDatabase<Realisation>::getInstance().getPositionId(classicalPosition)}) {
}

template<typename Realisation>
bool Superposition<Realisation>::empty() const {
    return realisationIds.empty();
}

template<typename Realisation>
void Superposition<Realisation>::addRealisationId(PositionId realisationId) {
    realisationIds.emplace(realisationId);
}

template<typename Realisation>
const std::vector<PositionId> Superposition<Realisation>::getRealisationIds() const {
    std::vector<PositionId> result;
    for (PositionId realisationId : realisationIds) result.push_back(realisationId);
    return result;
}

template<typename Realisation>
std::vector<typename Realisation::Piece> Superposition<Realisation>::getPieces(Player player) const {
    std::set<typename Realisation::Piece> pieces;
    for (PositionId realisationId : realisationIds) {
        for (typename Realisation::Piece piece : PositionDatabase<Realisation>::getInstance().getGame(realisationId).getPieces(player)) {
            pieces.insert(piece);
        }
    }
    return std::vector<typename Realisation::Piece>(pieces.begin(), pieces.end());
}

namespace std {
    template<typename Realisation>
    struct hash<Superposition<Realisation>> {
        size_t operator()(const Superposition<Realisation>& superposition) const {
            return std::hash<std::vector<PositionId>>()(superposition.getRealisationIds());
        }
    };
}

#endif // SUPERPOSITION_H
