#ifndef ADJACENCY_MATRIX_POSITION_H
#define ADJACENCY_MATRIX_POSITION_H

#include "ClassicalPosition.h"

class AdjacencyMatrixPosition : public Position {
public:
    AdjacencyMatrixPosition(size_t nodeCount);

    Position* clone() const override;
    size_t getNodeCount() const override;
    void increaseNodeCount(size_t count) override;
    void addPiece(Edge piece, PieceColour colour) override;
    bool removePiece(Edge piece) override;
    std::vector<Edge> getPieces(Player player) const override;
    PieceColour getPieceColour(Edge piece) const override;
    void setPieceColour(Edge piece, PieceColour colour) override;
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
