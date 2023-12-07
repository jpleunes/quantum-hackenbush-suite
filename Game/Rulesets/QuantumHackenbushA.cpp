#include "QuantumHackenbushA.h"

QuantumHackenbushA::QuantumHackenbushA(Position *position) : QuantumHackenbush(position) {
};

std::vector<QuantumHackenbush*> QuantumHackenbushA::getBlueOptions() const {
    return std::vector<QuantumHackenbush*>(); // TODO
};

std::vector<QuantumHackenbush*> QuantumHackenbushA::getRedOptions() const {
    return std::vector<QuantumHackenbush*>(); // TODO
};

OutcomeClass QuantumHackenbushA::determineOutcomeClass() const {
    return OutcomeClass::N; // TODO
};

QuantumHackenbushA::~QuantumHackenbushA() {
};
