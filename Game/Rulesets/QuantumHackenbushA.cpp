#include <stack>

#include "QuantumHackenbushA.h"

const int width = 2;

QuantumHackenbushA::QuantumHackenbushA(Position *position) : QuantumHackenbush(position) {
};

/// @brief TODO
/// @param n 
/// @return 
std::vector<std::vector<size_t>> indexCombinations(size_t n) {
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

std::vector<QuantumHackenbush*> QuantumHackenbushA::getBlueOptions() const {
    std::vector<QuantumHackenbush*> blueOptions;
    std::vector<Edge> bluePieces = position->getBluePieces();

    if (bluePieces.size() < width) return blueOptions;

    for (std::vector<size_t> combination : indexCombinations(bluePieces.size())) {
        for (size_t i = 0; i < position->getWidth(); i++) {
            Position *option = new Position(position->getRealisation(0).getNodeCount()); // TODO: find number of possible nodes (nodes may have been deleted in some realisations)
            for (size_t pieceIndex : combination) {
                ClassicalPosition *newRealisation = position->getRealisation(i).clone();
                bool removalSuccessful = newRealisation->removePiece(bluePieces[pieceIndex]);
                if (removalSuccessful) option->addRealisation(newRealisation);
            }
            blueOptions.push_back(new QuantumHackenbushA(option));
        }
    }
    return blueOptions;
};

std::vector<QuantumHackenbush*> QuantumHackenbushA::getRedOptions() const {
    std::vector<QuantumHackenbush*> redOptions;
    std::vector<Edge> redPieces = position->getRedPieces();

    if (redPieces.size() < width) return redOptions;

    for (std::vector<size_t> combination : indexCombinations(redPieces.size())) {
        for (size_t i = 0; i < position->getWidth(); i++) {
            Position *option = new Position(position->getRealisation(0).getNodeCount()); // TODO: find number of possible nodes (nodes may have been deleted in some realisations)
            for (size_t pieceIndex : combination) {
                ClassicalPosition *newRealisation = position->getRealisation(i).clone();
                bool removalSuccessful = newRealisation->removePiece(redPieces[pieceIndex]);
                if (removalSuccessful) option->addRealisation(newRealisation);
            }
            redOptions.push_back(new QuantumHackenbushA(option));
        }
    }
    return redOptions;
};

QuantumHackenbushA::~QuantumHackenbushA() {
    delete position;
};
