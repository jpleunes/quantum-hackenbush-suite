#ifndef ADJACENCY_MATRIX_POSITION_H
#define ADJACENCY_MATRIX_POSITION_H

#include <boost/container_hash/hash.hpp>

#include "GraphPosition.h"
#include "../../../../Util/HashUtil.h"

class AdjacencyMatrixPosition : public GraphPosition {
public:
    typedef Edge Piece;

    AdjacencyMatrixPosition();
    AdjacencyMatrixPosition(const AdjacencyMatrixPosition& other);
    AdjacencyMatrixPosition(size_t nodeCount);
    bool operator==(const AdjacencyMatrixPosition& other) const;

    void addPiece(Edge piece, PieceColour colour) override;
    bool removePiece(Edge piece) override;
    std::set<Edge> getPieces(Player player) const override;
    /// @brief Removes all pieces that are no longer connected to the ground.
    /// @param removedPiece piece that was last removed, can be used to stop the breadth-first search earlier
    void removeNotConnectedToGround(Edge removedPiece);
    /// @brief Removes all pieces that are no longer connected to the ground.
    void removeNotConnectedToGround();
    void printHumanReadable() const override;

    ~AdjacencyMatrixPosition() override = default;

    std::vector<std::vector<PieceColour>> adjacencyMatrix;
    
private:
    const NodeId groundId = 0;
};

namespace std {
    template<>
    struct hash<AdjacencyMatrixPosition> {
        size_t operator()(const AdjacencyMatrixPosition& position) const {
            size_t hash = 0;
            for (const std::vector<PieceColour>& row : position.adjacencyMatrix) {
                std::vector<uint32_t> intVec;
                for (PieceColour x : row) intVec.push_back((uint32_t) x);
                if (hash == 0) {
                    hash = std::hash<std::vector<uint32_t>>()(intVec);
                    continue;
                }
                boost::hash_combine(hash, std::hash<std::vector<uint32_t>>()(intVec));
            }

            return hash;
        }
    };
}

#endif // ADJACENCY_MATRIX_POSITION_H
