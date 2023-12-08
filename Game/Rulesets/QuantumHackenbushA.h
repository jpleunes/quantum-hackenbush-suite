#ifndef QUANTUM_HACKENBUSHA_H
#define QUANTUM_HACKENBUSHA_H

#include "../QuantumHackenbush.h"

class QuantumHackenbushA : public QuantumHackenbush {
public:
    QuantumHackenbushA(Position *position);
    std::vector<QuantumHackenbush*> getBlueOptions() const override;
    std::vector<QuantumHackenbush*> getRedOptions() const override;

    ~QuantumHackenbushA() override;
};

#endif // QUANTUM_HACKENBUSHA_H
