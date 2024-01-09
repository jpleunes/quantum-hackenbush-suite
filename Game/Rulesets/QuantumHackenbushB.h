#ifndef QUANTUM_HACKENBUSHB_H
#define QUANTUM_HACKENBUSHB_H

#include "../QuantumHackenbush.h"

class QuantumHackenbushB : public QuantumHackenbush {
public:
    QuantumHackenbushB(const Position *position);
    std::vector<QuantumHackenbush*> getOptions(Player player) const override;

    ~QuantumHackenbushB() override;
};

#endif // QUANTUM_HACKENBUSHB_H
