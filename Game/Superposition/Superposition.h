#ifndef SUPERPOSITION_H
#define SUPERPOSITION_H

#include <vector>
#include <set>
#include <unordered_set>

template<typename Realisation, typename Piece>
class Superposition {
public:
    Superposition() = default;
    Superposition(const Realisation classicalPosition);
    bool empty() const;
    void addRealisation(const Realisation realisation);
    const std::vector<const Realisation*> getRealisations() const;
    // Gets the pieces which exist in at least one realisation (and can thus be used in a move) 
    // for a given player.
    std::vector<Piece> getPieces(Player player) const;

    ~Superposition() = default;
    
private:
    std::unordered_set<Realisation> realisations;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation, typename Piece>
Superposition<Realisation, Piece>::Superposition(const Realisation classicalPosition) : realisations({classicalPosition}) {
}

template<typename Realisation, typename Piece>
bool Superposition<Realisation, Piece>::empty() const {
    return realisations.empty();
}

template<typename Realisation, typename Piece>
void Superposition<Realisation, Piece>::addRealisation(const Realisation realisation) {
    realisations.emplace(realisation);
}

template<typename Realisation, typename Piece>
const std::vector<const Realisation*> Superposition<Realisation, Piece>::getRealisations() const {
    std::vector<const Realisation*> result;
    for (const Realisation& realisation : realisations) result.push_back(&realisation);
    return result;
}

template<typename Realisation, typename Piece>
std::vector<Piece> Superposition<Realisation, Piece>::getPieces(Player player) const {
    std::set<Piece> pieces;
    for (const Realisation& realisation : realisations) {
        for (Piece piece : realisation.getPieces(player)) pieces.insert(piece);
    }
    return std::vector<Piece>(pieces.begin(), pieces.end());
}

#endif // SUPERPOSITION_H
