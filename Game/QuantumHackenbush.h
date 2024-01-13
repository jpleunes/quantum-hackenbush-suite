#ifndef QUANTUM_HACKENBUSH_H
#define QUANTUM_HACKENBUSH_H

#include "Position.h"
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
class QuantumHackenbush {
public:
    QuantumHackenbush(const Position *position);

    virtual Generator<QuantumHackenbush*> options(Player player) const = 0;
    OutcomeClass determineOutcomeClass() const;

    virtual ~QuantumHackenbush() = default;

protected:
    template<typename Ruleset>
    Generator<Ruleset*> superposedMoveOptions(const std::vector<Edge> &pieces) const { // TODO: also allow moves with width >2 (use width as the maximum width?)
        if (pieces.size() < width) co_return;
        auto indexCombinationsGen = indexCombinations(pieces.size());
        while (indexCombinationsGen) {
            std::vector<size_t> move = indexCombinationsGen();
            Position *option = new Position();
            for (size_t i = 0; i < position->getWidth(); i++) {
                for (size_t pieceIndex : move) {
                    ClassicalPosition *newRealisation = position->getRealisation(i).clone();
                    bool valid = newRealisation->removePiece(pieces[pieceIndex]);
                    if (valid) option->addRealisation(newRealisation);
                    else delete newRealisation;
                }
            }
            if (option->getWidth() > 0) co_yield new Ruleset(option);
            else delete option;
        }
    }

    const Position *position;

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

#endif // QUANTUM_HACKENBUSH_H
