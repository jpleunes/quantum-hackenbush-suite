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
    std::vector<QuantumHackenbush*> blueOptions = getBlueOptions();
    std::vector<OutcomeClass> blueOutcomes;
    for (auto blueOption : blueOptions) {
        blueOutcomes.push_back(blueOption->determineOutcomeClass());
        delete blueOption;
    }
    std::vector<QuantumHackenbush*> redOptions = getRedOptions();
    std::vector<OutcomeClass> redOutcomes;
    for (auto redOption : redOptions) {
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
