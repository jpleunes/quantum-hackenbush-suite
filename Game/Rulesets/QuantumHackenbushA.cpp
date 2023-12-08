#include <stack>

#include "QuantumHackenbushA.h"

#define WIDTH 2

QuantumHackenbushA::QuantumHackenbushA(Position *position) : QuantumHackenbush(position) {
};

/// @brief TODO
/// @param n 
/// @return 
std::vector<std::vector<size_t>> indexCombinations(size_t n) {
    std::vector<std::vector<size_t>> result;
    // This algorithm is based on https://codereview.stackexchange.com/a/195025
    std::vector<size_t> combination(WIDTH);
    std::stack<size_t> stack;
    stack.push(0);
    while (!stack.empty()) {
        size_t index = stack.size() - 1;
        size_t value = stack.top();
        stack.pop();
        while (value < n) {
            combination[index++] = value++;
            if (index != WIDTH) continue;
            std::vector<size_t> combinationCopy = combination;
            result.push_back(combinationCopy);
            break;
        }
    }
    return result;
}

std::vector<QuantumHackenbush*> QuantumHackenbushA::getBlueOptions() const {
    std::vector<QuantumHackenbush*> blueOptions;
    std::vector<Edge> bluePieces = position->getBluePieces();

    if (bluePieces.size() < WIDTH) return blueOptions;

    for (std::vector<size_t> combination : indexCombinations(bluePieces.size())) {
        for (size_t i = 0; i < position->getWidth(); i++) {
            Position *option = new Position(position->getRealisation(0).getNodeCount()); // TODO: find number of possible nodes (nodes may have been deleted in some realisations)
            for (size_t pieceIndex : combination) {
                ClassicalPosition *newRealisation = position->getRealisation(i).clone();
                newRealisation->removePiece(bluePieces[pieceIndex]);
                option->addRealisation(newRealisation);
            }
            blueOptions.push_back(new QuantumHackenbushA(option));
        }
    }
    return blueOptions;
};

std::vector<QuantumHackenbush*> QuantumHackenbushA::getRedOptions() const {
    std::vector<QuantumHackenbush*> redOptions;
    std::vector<Edge> redPieces = position->getRedPieces();

    if (redPieces.size() < WIDTH) return redOptions;

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
