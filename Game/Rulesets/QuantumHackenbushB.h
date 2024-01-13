#ifndef QUANTUM_HACKENBUSH_B_H
#define QUANTUM_HACKENBUSH_B_H

#include "../QuantumHackenbush.h"

class QuantumHackenbushB : public QuantumHackenbush {
public:
    QuantumHackenbushB(const Position *position);
    Generator<QuantumHackenbush*> options(Player player) const override;

    ~QuantumHackenbushB() override;
};

#endif // QUANTUM_HACKENBUSH_B_H
