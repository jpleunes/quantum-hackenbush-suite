#ifndef QUANTUM_HACKENBUSHC_H
#define QUANTUM_HACKENBUSHC_H

#include "../QuantumHackenbush.h"

class QuantumHackenbushC : public QuantumHackenbush {
public:
    QuantumHackenbushC(const Position *position);
    std::vector<QuantumHackenbush*> getOptions(Player player) const override;

    ~QuantumHackenbushC() override;
};

#endif // QUANTUM_HACKENBUSHC_H
