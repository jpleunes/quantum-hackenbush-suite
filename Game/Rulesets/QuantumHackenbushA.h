#ifndef QUANTUM_HACKENBUSH_A_H
#define QUANTUM_HACKENBUSH_A_H

#include "../QuantumHackenbush.h"

class QuantumHackenbushA : public QuantumHackenbush {
public:
    QuantumHackenbushA(const Superposition *superposition);
    Generator<QuantumHackenbush*> options(Player player) const override;

    ~QuantumHackenbushA() override;
};

#endif // QUANTUM_HACKENBUSH_A_H
