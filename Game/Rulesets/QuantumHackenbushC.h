#ifndef QUANTUM_HACKENBUSH_C_H
#define QUANTUM_HACKENBUSH_C_H

#include "../QuantumHackenbush.h"

class QuantumHackenbushC : public QuantumHackenbush {
public:
    QuantumHackenbushC(const Superposition *superposition);
    Generator<QuantumHackenbush*> options(Player player) const override;

    ~QuantumHackenbushC() override;
};

#endif // QUANTUM_HACKENBUSH_C_H
