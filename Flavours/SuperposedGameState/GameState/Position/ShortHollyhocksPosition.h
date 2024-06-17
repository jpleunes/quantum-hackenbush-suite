#ifndef SHORT_HOLLYHOCKS_POSITION_H
#define SHORT_HOLLYHOCKS_POSITION_H

#include "Position.h"
#include "../../../../Util/HashUtil.h"
#include "../../RealisationsUtil.h"

enum class ShortHollyhock : char {
    BLUE_HALF,
    RED_HALF,
    BLUE_WHOLE,
    RED_WHOLE,
    NONE
};

struct ShortHollyhocksCounts {
    bool operator==(const ShortHollyhocksCounts& other) const {
        return nBlueHalves == other.nBlueHalves && nRedHalves == other.nRedHalves && nBlueWholes == other.nBlueWholes && nRedWholes == other.nRedWholes;
    }

    bool operator<(const ShortHollyhocksCounts& other) const {
        return nBlueHalves < other.nBlueHalves || nRedHalves < other.nRedHalves || nBlueWholes < other.nBlueWholes || nRedWholes < other.nRedWholes;
    }

    size_t nBlueHalves = 0, nRedHalves = 0, nBlueWholes = 0, nRedWholes = 0;
};

namespace std {
    template<>
    struct hash<ShortHollyhocksCounts> {
        size_t operator()(const ShortHollyhocksCounts& counts) const {
            std::vector<uint32_t> intVec;
            intVec.push_back((uint32_t) counts.nBlueHalves);
            intVec.push_back((uint32_t) counts.nRedHalves);
            intVec.push_back((uint32_t) counts.nBlueWholes);
            intVec.push_back((uint32_t) counts.nRedWholes);
            return std::hash<std::vector<uint32_t>>()(intVec);
        }
    };
}

class ShortHollyhocksPosition : public Position<ShortHollyhocksPiece, ShortHollyhocksRealisations> {
public:
    typedef ShortHollyhocksPiece Piece;

    ShortHollyhocksPosition();
    ShortHollyhocksPosition(const ShortHollyhocksPosition& other);
    bool operator==(const ShortHollyhocksPosition& other) const;

    void addPiece(ShortHollyhocksPiece piece);
    bool removePiece(ShortHollyhocksPiece piece) override;
    const std::vector<ShortHollyhock>& getShortHollyhocks() const;
    std::set<ShortHollyhocksPiece> getPieces(Player player) const override;
    void printHumanReadable() const override;
    const ShortHollyhocksCounts getCounts() const;

    ~ShortHollyhocksPosition() override = default;

    std::vector<ShortHollyhock> shortHollyhocks;
};

namespace std {
    template<>
    struct hash<ShortHollyhocksPosition> {
        size_t operator()(const ShortHollyhocksPosition& position) const {
            std::vector<uint32_t> intVec;
            for (ShortHollyhock x : position.shortHollyhocks) intVec.push_back((uint32_t) x);
            return std::hash<std::vector<uint32_t>>()(intVec);
        }
    };
}

// Prevent recursive dependence
#include "../../Realisations.h"

#endif // SHORT_HOLLYHOCKS_POSITION_H
