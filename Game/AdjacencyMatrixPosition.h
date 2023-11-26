#ifndef ADJACENCY_MATRIX_POSITION_H
#define ADJACENCY_MATRIX_POSITION_H

#include "Position.h"

class AdjacencyMatrixPosition : public ClassicalPosition {
public:
    AdjacencyMatrixPosition(size_t nodeCount);

    ClassicalPosition* clone() const override;
    size_t getNodeCount() const override;
    void increaseNodeCount(size_t count) override;
    void addPiece(NodeId from, NodeId to, PieceColour color) override;
    void removePiece(NodeId from, NodeId to) override;
    std::vector<Edge> getBluePieces() const override;
    std::vector<Edge> getRedPieces() const override;

    ~AdjacencyMatrixPosition() override;
    
private:
    std::vector<std::vector<PieceColour>> adjacencyMatrix;
    const NodeId groundId = 0;
};

#endif // ADJACENCY_MATRIX_POSITION_H
