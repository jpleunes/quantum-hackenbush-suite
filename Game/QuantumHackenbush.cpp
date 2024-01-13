#include <stdexcept>

#include "QuantumHackenbush.h"

QuantumHackenbush::QuantumHackenbush(const Position *position) : position(position) {
}

OutcomeClass QuantumHackenbush::determineOutcomeClass() const {
    auto leftStartOutcome = determineOutcomeClass(Player::LEFT);
    auto rightStartOutcome = determineOutcomeClass(Player::RIGHT);

    // We do not need to check whether both left and right can win when they start, because such 
    // situations are impossible (Hackenbush does not have N-positions).
    if (leftStartOutcome == OutcomeClass::L || leftStartOutcome == OutcomeClass::P) return OutcomeClass::L;
    else if (rightStartOutcome == OutcomeClass::R || rightStartOutcome == OutcomeClass::P) return OutcomeClass::R;
    else return OutcomeClass::P;
}

OutcomeClass QuantumHackenbush::determineOutcomeClass(Player turn) const {
    int nodesCount = 1;

    std::vector<QuantumHackenbush*> options = getOptions(turn);
    switch (turn) {
        case Player::LEFT:
            for (QuantumHackenbush *option : options) {
                OutcomeClass outcome = option->determineOutcomeClass(Player::RIGHT);
                if (outcome == OutcomeClass::L || outcome == OutcomeClass::P) {
                    for (QuantumHackenbush *o : options) delete o;
                    return OutcomeClass::L;
                }
            }

            for (QuantumHackenbush *o : options) delete o;
            return OutcomeClass::R;
        case Player::RIGHT:
            for (QuantumHackenbush *option : options) {
                OutcomeClass outcome = option->determineOutcomeClass(Player::LEFT);
                if (outcome == OutcomeClass::R || outcome == OutcomeClass::P) {
                    for (QuantumHackenbush *o : options) delete o;
                    return OutcomeClass::R;
                }
            }

            for (QuantumHackenbush *o : options) delete o;
            return OutcomeClass::L;
        default:
            throw(std::domain_error("Unknown player case"));
    }
};
