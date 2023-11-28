#include <queue>

#include "Position.h"

AdjacencyMatrixPosition::AdjacencyMatrixPosition(size_t nodeCount) {
    adjacencyMatrix.resize(nodeCount);
    // A Hackenbush position is represented as an undirected graph, so we only store the lower triangle of the adjacency matrix.
    for (size_t i = 0; i < nodeCount; i++) {
        adjacencyMatrix[i].resize(i + 1, PieceColour::NONE);
    }
}

ClassicalPosition* AdjacencyMatrixPosition::clone() const {
    AdjacencyMatrixPosition* clone = new AdjacencyMatrixPosition(adjacencyMatrix.size());
    for (size_t i = 0; i < adjacencyMatrix.size(); i++) {
        for (size_t j = 0; j < adjacencyMatrix[i].size(); j++) {
            clone->adjacencyMatrix[i][j] = adjacencyMatrix[i][j];
        }
    }
    return clone;
}

size_t AdjacencyMatrixPosition::getNodeCount() const {
    return adjacencyMatrix.size();
}

void AdjacencyMatrixPosition::increaseNodeCount(size_t count) {
    // TODO: check if count is smaller than current count
    adjacencyMatrix.resize(count);
    for (size_t i = 0; i < count; i++) {
        adjacencyMatrix[i].resize(count - i, PieceColour::NONE);
    }
}

void AdjacencyMatrixPosition::addPiece(NodeId from, NodeId to, PieceColour color) {
    if (from < to) {
        adjacencyMatrix[to][from] = color;
    }
    else {
        adjacencyMatrix[from][to] = color;
    }
    // TODO: check if this piece is connected to ground?
}

void AdjacencyMatrixPosition::removePiece(NodeId from, NodeId to) {
    if (from < to) {
        adjacencyMatrix[to][from] = PieceColour::NONE;
    }
    else {
        adjacencyMatrix[from][to] = PieceColour::NONE;
    }

    // Remove all pieces which are no longer connected to ground
    // TODO: remove unreachable nodes by resizing matrix (for this we need node id translations)
    std::vector<bool> visited(adjacencyMatrix.size(), false);
    std::queue<NodeId> queue;
    queue.push(groundId);
    bool firstVisited = false;
    bool secondVisited = false;
    while (!queue.empty()) {
        NodeId current = queue.front();
        queue.pop();
        visited[current] = true;
        firstVisited |= current == from;
        secondVisited |= current == to;
        if (firstVisited && secondVisited) return;
        for (size_t neighbour = 0; neighbour < adjacencyMatrix.size(); neighbour++) {
            const PieceColour pieceColour = (current < neighbour) ? adjacencyMatrix[neighbour][current] : adjacencyMatrix[current][neighbour];
            if (pieceColour != PieceColour::NONE && !visited[neighbour]) {
                queue.push(neighbour);
            }
        }
    }
    // Once we have all reachable nodes, we remove all pieces connected to unreachable nodes
    for (size_t i = 0; i < adjacencyMatrix.size(); i++) {
        if (visited[i]) continue;
        for (size_t j = 0; j < adjacencyMatrix.size(); j++) {
            if (i < j) {
                adjacencyMatrix[j][i] = PieceColour::NONE;
            }
            else {
                adjacencyMatrix[i][j] = PieceColour::NONE;
            }
        }
    }
}

std::vector<Edge> AdjacencyMatrixPosition::getBluePieces() const {
    std::vector<Edge> bluePieces;
    for (size_t i = 0; i < adjacencyMatrix.size(); i++) {
        for (size_t j = 0; j < adjacencyMatrix[i].size(); j++) {
            if (adjacencyMatrix[i][j] == PieceColour::BLUE) {
                bluePieces.push_back(std::make_pair(i, j));
            }
        }
    }
    return bluePieces;
}

std::vector<Edge> AdjacencyMatrixPosition::getRedPieces() const {
    std::vector<Edge> redPieces;
    for (size_t i = 0; i < adjacencyMatrix.size(); i++) {
        for (size_t j = 0; j < adjacencyMatrix[i].size(); j++) {
            if (adjacencyMatrix[i][j] == PieceColour::RED) {
                redPieces.push_back(std::make_pair(i, j));
            }
        }
    }
    return redPieces;
}

PieceColour AdjacencyMatrixPosition::getPieceColour(Edge piece) const {
    if (piece.first < piece.second) {
        return adjacencyMatrix[piece.second][piece.first];
    }
    else {
        return adjacencyMatrix[piece.first][piece.second];
    }
}

// TODO: move to Matrix class
void AdjacencyMatrixPosition::setPieceColour(Edge piece, PieceColour pieceColour) {
    if (piece.first < piece.second) {
        adjacencyMatrix[piece.second][piece.first] = pieceColour;
    }
    else {
        adjacencyMatrix[piece.first][piece.second] = pieceColour;
    }
}

AdjacencyMatrixPosition::~AdjacencyMatrixPosition() {
}
