#ifndef QUANTUM_HACKENBUSH_A_H
#define QUANTUM_HACKENBUSH_A_H

#include "../QuantumHackenbush.h"

template<typename Realisation>
class QuantumHackenbushA : public QuantumHackenbush<Realisation> {
public:
    QuantumHackenbushA(const Superposition<Realisation> superposition);
    bool operator==(const QuantumHackenbushA<Realisation>& other) const;
    std::vector<GameInstanceId> getMoveOptions(Player player) const override;

    ~QuantumHackenbushA() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
QuantumHackenbushA<Realisation>::QuantumHackenbushA(const Superposition<Realisation> superposition) : QuantumHackenbush<Realisation>(superposition) {
};

template<typename Realisation>
bool QuantumHackenbushA<Realisation>::operator==(const QuantumHackenbushA<Realisation>& other) const {
    return this->superposition == other.superposition;
}

template<typename Realisation>
std::vector<GameInstanceId> QuantumHackenbushA<Realisation>::getMoveOptions(Player player) const {
    return getSuperposedMoveOptions<QuantumHackenbushA<Realisation>>(player);
};

namespace std {
    template<typename Realisation>
    struct hash<QuantumHackenbushA<Realisation>> {
        size_t operator()(const QuantumHackenbushA<Realisation>& quantumHackenbush) const {
            return std::hash<Superposition<Realisation>>()(quantumHackenbush.getSuperposition());
        }
    };
}

#endif // QUANTUM_HACKENBUSH_A_H
