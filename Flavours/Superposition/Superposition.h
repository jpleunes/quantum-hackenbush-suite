#ifndef SUPERPOSITION_H
#define SUPERPOSITION_H

#include <vector>
#include <set>
#include <limits>
#include <algorithm>

#include "../../CombinatorialGame/ShortGame.h"
#include "Position/HackenbushDatabase.h"
#include "../../Util/HashUtil.h"
#include "Generator.h"

const int width = 2;

typedef size_t SuperpositionId;

#include "SuperpositionDatabase.h" // This include needs to be here, because it depends on SuperpositionId

struct SuperpositionCache {
    std::optional<std::vector<SuperpositionId>> leftOptions, rightOptions;
    std::optional<ShortGameId> shortGameId;
    // TODO: displayString
};

/**
 * An abstract class representing a Quantum Hackenbush superposition under some flavour.
 */
template<typename Realisation>
class Superposition {
public:
    // This constructor should only be used from within SuperpositionDatabase.
    // Otherwise, SuperpositionDatabase.getOrInsert should be used.
    Superposition(HackenbushId classicalPosition, SuperpositionId id);
    // This constructor should only be used from within SuperpositionDatabase.
    // Otherwise, SuperpositionDatabase.getOrInsert should be used.
    Superposition(std::set<HackenbushId> realisations, SuperpositionId id);

    const std::set<HackenbushId>& getRealisations() const { return realisations; }
    SuperpositionId getId() const { return id; }
    // Checks whether two superpositions have equal realisations sets.
    bool operator==(const Superposition<Realisation>& other) const {
        // Superpositions with equal realisations sets are assigned the same id.
        // Two realisations are considered equal if the repersentations of the positions are equal.
        // Note that equality of position representanions implies label-isomorphism.
        return id == other.id;
    }
    // Gets the pieces which exist in at least one realisation (and can thus be used in a move) 
    // for a given player.
    std::vector<typename Realisation::Piece> getPieces(Player player) const;

    virtual std::vector<SuperpositionId> getOptions(Player player) const = 0;

    // Inspired by Xander Lenstra https://github.com/xlenstra/CGSynch.
    template<typename Flavour>
    ShortGameId determineShortGameId() const {
        if (cache.shortGameId.has_value()) return cache.shortGameId.value();

        std::set<ShortGameId> leftOptions, rightOptions;
        for (SuperpositionId leftOption : getOptions(Player::LEFT)) {
            leftOptions.insert(SuperpositionDatabase<Flavour>::getInstance().getSuperposition(leftOption).template determineShortGameId<Flavour>());
        }
        for (SuperpositionId rightOption : getOptions(Player::RIGHT)) {
            rightOptions.insert(SuperpositionDatabase<Flavour>::getInstance().getSuperposition(rightOption).template determineShortGameId<Flavour>());
        }
        ShortGameId result = ShortGameDatabase::getInstance().getOrInsert(leftOptions, rightOptions).getId();

        cache.shortGameId = result;

        return result;
    }

    virtual ~Superposition() = default;

protected:
    template<typename Flavour>
    std::vector<SuperpositionId> getSuperposedOptions(Player player) const { // TODO: also allow moves with width >2 (use width as the maximum width?)
        std::vector<typename Realisation::Piece> pieces = getPieces(player);

        std::vector<SuperpositionId> result;
        if (pieces.size() < width) return result;
        auto indexCombinationsGen = indexCombinations(pieces.size());
        while (indexCombinationsGen) {
            std::vector<size_t> move = indexCombinationsGen();
            std::set<HackenbushId> option;
            for (HackenbushId realisationId : realisations) {
                for (size_t pieceIndex : move) {
                    Hackenbush<Realisation> realisation = HackenbushDatabase<Realisation>::getInstance().getPosition(realisationId);
                    std::optional<HackenbushId> newRealisationId = realisation.applyMove(pieces[pieceIndex]);
                    if (newRealisationId.has_value()) option.insert(newRealisationId.value());
                }
            }
            if (!option.empty()) {
                SuperpositionId superpositionId = SuperpositionDatabase<Flavour>::getInstance().getOrInsert(option).getId();
                result.emplace_back(superpositionId);
            }
        }
        return result;
    }

    mutable SuperpositionCache cache;
    
private:
    const std::set<HackenbushId> realisations;
    const SuperpositionId id;

    /// @brief TODO
    /// @param n 
    /// @return 
    Generator<std::vector<size_t>> indexCombinations(size_t n) const {
        // This algorithm is a C++ adaptation of https://github.com/blazs/subsets
        std::vector<size_t> combination(width);
        int i, j, r;

        for (i = 0; i < width; ++i) combination[i] = i; // Initial combination
        while (true) {
            std::vector<size_t> combinationCopy = combination;
            co_yield combinationCopy;

            if (combination[0] == n - width) break;

            for (i = width - 1; i >= 0 && combination[i] + width - i == n; --i);
            r = combination[i];
            ++combination[i];
            j = 2;
            for (++i; i < width; ++i, ++j) combination[i] = r + j;
        }
    }
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
Superposition<Realisation>::Superposition(HackenbushId classicalPosition, SuperpositionId id) : realisations({classicalPosition}), id(id) {
}

template<typename Realisation>
Superposition<Realisation>::Superposition(std::set<HackenbushId> realisations, SuperpositionId id) : realisations(realisations), id(id) {
    if (realisations.empty()) throw(std::domain_error("Cannot construct empty superposition."));
}

template<typename Realisation>
std::vector<typename Realisation::Piece> Superposition<Realisation>::getPieces(Player player) const {
    std::set<typename Realisation::Piece> pieces;
    for (HackenbushId realisationId : realisations) {
        for (typename Realisation::Piece piece : HackenbushDatabase<Realisation>::getInstance().getPosition(realisationId).getPieces(player)) {
            pieces.insert(piece);
        }
    }
    return std::vector<typename Realisation::Piece>(pieces.begin(), pieces.end());
}

#endif // SUPERPOSITION_H
