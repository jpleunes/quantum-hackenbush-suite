#include "QuantumHackenbush.h"

QuantumHackenbush::QuantumHackenbush(const Position *position) : position(position) {
}

OutcomeClass QuantumHackenbush::determineOutcomeClass() const { // TODO: count nodes to get an idea of the size of the tree
    if (position->empty()) {
        // We move to an empty superposition from a losing position->
        // This means that the empty superposition has outcome class N, 
        // which is the opposite of the outcome class of a losing position (P).
        return OutcomeClass::N;
    }

    // We are not in any of the base cases, so we need to use recursion.
    std::vector<OutcomeClass> blueOutcomes, redOutcomes;
    for (auto blueOption : getBlueOptions()) {
        blueOutcomes.push_back(blueOption->determineOutcomeClass());
        delete blueOption;
    }
    for (auto redOption : getRedOptions()) {
        redOutcomes.push_back(redOption->determineOutcomeClass());
        delete redOption;
    }

    // TODO: possible improvement: only calculate outcome when needed
    for (OutcomeClass blueOutcome : blueOutcomes) {
        if (blueOutcome == OutcomeClass::L || blueOutcome == OutcomeClass::P) {
            for (OutcomeClass redOutcome : redOutcomes) {
                if (redOutcome == OutcomeClass::R || redOutcome == OutcomeClass::P) {
                    return OutcomeClass::N;
                }
            }
            return OutcomeClass::L;
        }
    }
    for (OutcomeClass redOutcome : redOutcomes) {
        if (redOutcome == OutcomeClass::R || redOutcome == OutcomeClass::P) {
            return OutcomeClass::R;
        }
    }
    return OutcomeClass::P;
};

/// @brief TODO
/// @param n 
/// @return 
std::vector<std::vector<size_t>> QuantumHackenbush::indexCombinations(size_t n) const {
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
