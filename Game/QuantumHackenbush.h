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

    virtual std::vector<QuantumHackenbush*> getBlueOptions() const = 0; // TODO: use IEnumerable with yield for better performance
    virtual std::vector<QuantumHackenbush*> getRedOptions() const = 0;
    OutcomeClass determineOutcomeClass() const;

    virtual ~QuantumHackenbush() = default;

protected:
    template<typename Ruleset>
    std::vector<Ruleset*> getBlueSuperposedMoveOptions(const std::vector<Edge> &bluePieces) const {
        std::vector<Ruleset*> blueOptions;

        if (bluePieces.size() < width) return blueOptions;
        for (std::vector<size_t> move : indexCombinations(bluePieces.size())) {
            Position *option = new Position();
            for (size_t i = 0; i < position->getWidth(); i++) {
                for (size_t pieceIndex : move) {
                    ClassicalPosition *newRealisation = position->getRealisation(i).clone();
                    bool valid = newRealisation->removePiece(bluePieces[pieceIndex]);
                    if (valid) option->addRealisation(newRealisation);
                    else delete newRealisation;
                }
            }
            if (option->getWidth() > 0) blueOptions.push_back(new Ruleset(option));
            else delete option;
        }

        return blueOptions;
    }

    template<typename Ruleset>
    std::vector<Ruleset*> getRedSuperposedMoveOptions(const std::vector<Edge> &redPieces) const {
        std::vector<Ruleset*> redOptions;

        if (redPieces.size() < width) return redOptions;
        for (std::vector<size_t> move : indexCombinations(redPieces.size())) {
            Position *option = new Position();
            for (size_t i = 0; i < position->getWidth(); i++) {
                for (size_t pieceIndex : move) {
                    ClassicalPosition *newRealisation = position->getRealisation(i).clone();
                    bool valid = newRealisation->removePiece(redPieces[pieceIndex]);
                    if (valid) option->addRealisation(newRealisation);
                    else delete newRealisation;
                }
            }
            if (option->getWidth() > 0) redOptions.push_back(new Ruleset(option));
            else delete option;
        }

        return redOptions;
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
