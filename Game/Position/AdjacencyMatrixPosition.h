#ifndef ADJACENCY_MATRIX_POSITION_H
#define ADJACENCY_MATRIX_POSITION_H

#include "Position.h"

class AdjacencyMatrixPosition : public Position<Edge> {
public:
    AdjacencyMatrixPosition(size_t nodeCount);

    void addPiece(Edge piece, PieceColour colour) override;
    std::vector<Edge> getPieces(Player player) const override;
    Position* applyMove(Edge piece) const override;
    /// @brief Removes all pieces that are no longer connected to the ground.
    /// @param removedPiece piece that was last removed, can be used to stop the breadth-first search earlier
    void removeNotConnectedToGround(Edge removedPiece);
    /// @brief Removes all pieces that are no longer connected to the ground.
    void removeNotConnectedToGround();
    void printHumanReadable() const override;

    ~AdjacencyMatrixPosition() override = default;
    
private:
    std::vector<std::vector<PieceColour>> adjacencyMatrix;
    const NodeId groundId = 0;
};

#endif // ADJACENCY_MATRIX_POSITION_H
