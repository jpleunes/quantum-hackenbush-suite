#ifndef RESTRICTED_POSITION_H
#define RESTRICTED_POSITION_H

#include "Position.h"
#include "../../Util/HashUtil.h"

enum class HalfOrWhole : char {
    BLUE_HALF,
    RED_HALF,
    BLUE_WHOLE,
    RED_WHOLE,
    NONE
};

class RestrictedPosition : public Position<RestrictedPiece> {
public:
    typedef RestrictedPiece Piece;

    RestrictedPosition();
    RestrictedPosition(const RestrictedPosition& other);
    bool operator==(const RestrictedPosition& other) const;

    void addPiece(RestrictedPiece piece);
    std::vector<RestrictedPiece> getPieces(Player player) const override;
    PositionId applyMove(RestrictedPiece piece) const override;
    void printHumanReadable() const override;

    ~RestrictedPosition() override = default;

    std::vector<HalfOrWhole> entries;
};

namespace std {
    template<>
    struct hash<RestrictedPosition> {
        size_t operator()(const RestrictedPosition& position) const {
            std::vector<uint32_t> intVec;
            for (HalfOrWhole x : position.entries) intVec.push_back((uint32_t) x);
            return std::hash<std::vector<uint32_t>>()(intVec);
        }
    };
}

#endif // RESTRICTED_POSITION_H
