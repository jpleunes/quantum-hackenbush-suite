#ifndef QUANTUM_HACKENBUSHB_H
#define QUANTUM_HACKENBUSHB_H

#include "../QuantumHackenbush.h"

class QuantumHackenbushB : public QuantumHackenbush {
public:
    QuantumHackenbushB(Position *position);
    std::vector<QuantumHackenbush*> getBlueOptions() const override;
    std::vector<QuantumHackenbush*> getRedOptions() const override;

    ~QuantumHackenbushB() override;
};

#endif // QUANTUM_HACKENBUSHB_H
