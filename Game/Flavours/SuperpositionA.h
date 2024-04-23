#ifndef SUPERPOSITION_A_H
#define SUPERPOSITION_A_H

#include "../Superposition/Superposition.h"

template<typename Realisation>
class SuperpositionA : public Superposition<Realisation> {
public:
    SuperpositionA() = default;
    SuperpositionA(PositionId positionId);
    std::vector<SuperpositionId> getMoveOptions(Player player) const override;

    ~SuperpositionA() override = default;
};

// This is a templated class, so the implementations need to go here

template<typename Realisation>
SuperpositionA<Realisation>::SuperpositionA(PositionId positionId) : Superposition<Realisation>(positionId) {
};

template<typename Realisation>
std::vector<SuperpositionId> SuperpositionA<Realisation>::getMoveOptions(Player player) const {
    return getSuperposedMoveOptions<SuperpositionA<Realisation>>(player);
};

namespace std {
    template<typename Realisation>
    struct hash<SuperpositionA<Realisation>> {
        size_t operator()(const SuperpositionA<Realisation>& superposition) const {
            return std::hash<Superposition<Realisation>>()(superposition);
        }
    };
}

#endif // SUPERPOSITION_A_H
