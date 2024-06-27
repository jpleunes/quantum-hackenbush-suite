#ifndef HACKENBUSH_STRING_POSITION_H
#define HACKENBUSH_STRING_POSITION_H

#include "Position.h"
#include "../../../../Util/HashUtil.h"

class HackenbushStringPosition : public Position<HackenbushStringPiece> {
public:
    typedef HackenbushStringPiece Piece;

    HackenbushStringPosition();
    HackenbushStringPosition(const HackenbushStringPosition& other);
    bool operator==(const HackenbushStringPosition& other) const;

    void addPiece(PieceColour colour);
    bool removePiece(HackenbushStringPiece piece) override;
    std::set<HackenbushStringPiece> getPieces(Player player) const override;
    void printHumanReadable() const override;

    ~HackenbushStringPosition() override = default;

    std::vector<PieceColour> hackenbushString;
};

namespace std {
    template<>
    struct hash<HackenbushStringPosition> {
        size_t operator()(const HackenbushStringPosition& position) const {
            std::vector<uint32_t> intVec;
            for (PieceColour x : position.hackenbushString) intVec.push_back((uint32_t) x);
            return std::hash<std::vector<uint32_t>>()(intVec);
        }
    };
}

#endif // HACKENBUSH_STRING_POSITION_H
