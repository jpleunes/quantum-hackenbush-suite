#ifndef QUANTUM_HACKENBUSH_D_H
#define QUANTUM_HACKENBUSH_D_H

#include "../QuantumHackenbush.h"

class QuantumHackenbushD : public QuantumHackenbush {
public:
    QuantumHackenbushD(const Superposition *superposition);
    Generator<QuantumHackenbush*> options(Player player) const override;

    ~QuantumHackenbushD() override;
};

#endif // QUANTUM_HACKENBUSH_D_H
