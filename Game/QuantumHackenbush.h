#ifndef QUANTUM_HACKENBUSH_H
#define QUANTUM_HACKENBUSH_H

#include "Superposition/Superposition.h"
#include "../Util/Generator.h"

const int width = 2;

enum class OutcomeClass {
    L,
    R,
    N,
    P,
};

/**
 * An abstract class representing a ruleset of Quantum Hackenbush.
 */
template<typename Realisation, typename Piece>
class QuantumHackenbush {
public:
    QuantumHackenbush(const Superposition<Realisation, Piece> superposition);

    virtual Generator<QuantumHackenbush<Realisation, Piece>*> options(Player player) const = 0;
    OutcomeClass determineOutcomeClass() const;

    virtual ~QuantumHackenbush() = default;

protected:
    template<typename Ruleset>
    Generator<Ruleset*> superposedMoveOptions(const std::vector<Piece> &pieces) const { // TODO: also allow moves with width >2 (use width as the maximum width?)
        if (pieces.size() < width) co_return;
        auto indexCombinationsGen = indexCombinations(pieces.size());
        while (indexCombinationsGen) {
            std::vector<size_t> move = indexCombinationsGen();
            Superposition<Realisation, Piece> option = Superposition<Realisation, Piece>();
            for (const Realisation* realisation : superposition.getRealisations()) {
                for (size_t pieceIndex : move) {
                    auto* newRealisationPtr = realisation->applyMove(pieces[pieceIndex]);
                    if (newRealisationPtr != nullptr) {
                        Realisation newRealisation = std::move(*static_cast<Realisation*>(newRealisationPtr));
                        delete newRealisationPtr;
                        option.addRealisation(newRealisation);
                    }
                }
            }
            if (!option.empty()) co_yield new Ruleset(option);
        }
    }

    const Superposition<Realisation, Piece> superposition;

private:
    OutcomeClass determineOutcomeClass(Player turn) const;

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

template<typename Realisation, typename Piece>
QuantumHackenbush<Realisation, Piece>::QuantumHackenbush(const Superposition<Realisation, Piece> superposition) : superposition(superposition) {
}

template<typename Realisation, typename Piece>
OutcomeClass QuantumHackenbush<Realisation, Piece>::determineOutcomeClass() const {
    auto leftStartOutcome = determineOutcomeClass(Player::LEFT);
    auto rightStartOutcome = determineOutcomeClass(Player::RIGHT);

    // We do not need to check whether both left and right can win when they start, because such 
    // situations are impossible (Hackenbush does not have N-positions).
    if (leftStartOutcome == OutcomeClass::L || leftStartOutcome == OutcomeClass::P) return OutcomeClass::L;
    else if (rightStartOutcome == OutcomeClass::R || rightStartOutcome == OutcomeClass::P) return OutcomeClass::R;
    else return OutcomeClass::P;
}

template<typename Realisation, typename Piece>
OutcomeClass QuantumHackenbush<Realisation, Piece>::determineOutcomeClass(Player turn) const {
    Generator<QuantumHackenbush<Realisation, Piece>*> optionsGen = options(turn);
    switch (turn) {
        case Player::LEFT:
            while (optionsGen) {
                QuantumHackenbush<Realisation, Piece> *option = optionsGen();
                OutcomeClass outcome = option->determineOutcomeClass(Player::RIGHT);
                delete option;
                if (outcome == OutcomeClass::L || outcome == OutcomeClass::P) return OutcomeClass::L;
            }
            return OutcomeClass::R;
        case Player::RIGHT:
            while (optionsGen) {
                QuantumHackenbush<Realisation, Piece> *option = optionsGen();
                OutcomeClass outcome = option->determineOutcomeClass(Player::LEFT);
                delete option;
                if (outcome == OutcomeClass::R || outcome == OutcomeClass::P) return OutcomeClass::R;
            }
            return OutcomeClass::L;
        default:
            throw(std::domain_error("Unknown player case."));
    }
};

#endif // QUANTUM_HACKENBUSH_H
