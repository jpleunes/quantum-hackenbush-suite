#ifndef QUANTUM_HACKENBUSH_H
#define QUANTUM_HACKENBUSH_H

#include <vector>

#include "Position.h"

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

    virtual std::vector<QuantumHackenbush*> getOptions(Player player) const = 0; // TODO: use IEnumerable with yield for better performance
    OutcomeClass determineOutcomeClass() const;

    virtual ~QuantumHackenbush() = default;

protected:
    template<typename Ruleset>
    std::vector<Ruleset*> getSuperposedMoveOptions(const std::vector<Edge> &pieces) const { // TODO: also allow moves with width >2
        std::vector<Ruleset*> options;

        if (pieces.size() < width) return options;
        for (std::vector<size_t> move : indexCombinations(pieces.size())) {
            Position *option = new Position();
            for (size_t i = 0; i < position->getWidth(); i++) {
                for (size_t pieceIndex : move) {
                    ClassicalPosition *newRealisation = position->getRealisation(i).clone();
                    bool valid = newRealisation->removePiece(pieces[pieceIndex]);
                    if (valid) option->addRealisation(newRealisation);
                    else delete newRealisation;
                }
            }
            if (option->getWidth() > 0) options.push_back(new Ruleset(option));
            else delete option;
        }

        return options;
    }

    const Position *position;

private:
    /// @brief TODO
    /// @param n 
    /// @return 
    std::vector<std::vector<size_t>> indexCombinations(size_t n) const {
        std::vector<std::vector<size_t>> result;
        // This algorithm is a C++ adaptation of https://github.com/blazs/subsets
        std::vector<size_t> combination(width);
        int i, j, r;

        for (i = 0; i < width; ++i) combination[i] = i; // Initial combination
        while (true) {
            // TODO: improve efficiency by using yield
            std::vector<size_t> combinationCopy = combination;
            result.push_back(combinationCopy);

            if (combination[0] == n - width) break;

            for (i = width - 1; i >= 0 && combination[i] + width - i == n; --i);
            r = combination[i];
            ++combination[i];
            j = 2;
            for (++i; i < width; ++i, ++j) combination[i] = r + j;
        }

        return result;
    }
};

#endif // QUANTUM_HACKENBUSH_H
