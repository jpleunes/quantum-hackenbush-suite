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
    std::vector<OutcomeClass> leftOutcomes, rightOutcomes;
    for (auto leftOption : getOptions(Player::LEFT)) {
        leftOutcomes.push_back(leftOption->determineOutcomeClass());
        delete leftOption;
    }
    for (auto rightOption : getOptions(Player::RIGHT)) {
        rightOutcomes.push_back(rightOption->determineOutcomeClass());
        delete rightOption;
    }

    // TODO: possible improvement: only calculate outcome when needed
    for (OutcomeClass leftOutcome : leftOutcomes) {
        if (leftOutcome == OutcomeClass::L || leftOutcome == OutcomeClass::P) {
            for (OutcomeClass rightOutcome : rightOutcomes) {
                if (rightOutcome == OutcomeClass::R || rightOutcome == OutcomeClass::P) {
                    return OutcomeClass::N;
                }
            }
            return OutcomeClass::L;
        }
    }
    for (OutcomeClass rightOutcome : rightOutcomes) {
        if (rightOutcome == OutcomeClass::R || rightOutcome == OutcomeClass::P) {
            return OutcomeClass::R;
        }
    }
    return OutcomeClass::P;
};
