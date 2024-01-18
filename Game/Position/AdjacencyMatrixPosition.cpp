#include <queue>
#include <iostream>

#include "AdjacencyMatrixPosition.h"

AdjacencyMatrixPosition::AdjacencyMatrixPosition(size_t nodeCount) {
    adjacencyMatrix.resize(nodeCount);
    // A Hackenbush position is represented as an undirected graph, so we only store the lower triangle of the adjacency matrix.
    for (size_t i = 0; i < nodeCount; i++) {
        adjacencyMatrix[i].resize(i + 1, PieceColour::NONE);
    }
}

Position* AdjacencyMatrixPosition::clone() const {
    AdjacencyMatrixPosition* clone = new AdjacencyMatrixPosition(adjacencyMatrix.size());
    for (size_t i = 0; i < adjacencyMatrix.size(); i++) {
        for (size_t j = 0; j < adjacencyMatrix[i].size(); j++) {
            clone->adjacencyMatrix[i][j] = adjacencyMatrix[i][j];
        }
    }
    return clone;
}

void AdjacencyMatrixPosition::addPiece(Edge piece, PieceColour colour) {
    if (piece.first < piece.second) {
        adjacencyMatrix[piece.second][piece.first] = colour;
    }
    else {
        adjacencyMatrix[piece.first][piece.second] = colour;
    }
    // TODO: check if this piece is connected to ground?
}

bool AdjacencyMatrixPosition::removePiece(Edge piece) {
    bool removeSuccessful = false;
    if (piece.first < piece.second) {
        if (adjacencyMatrix[piece.second][piece.first] != PieceColour::NONE) removeSuccessful = true;
        adjacencyMatrix[piece.second][piece.first] = PieceColour::NONE;
    }
    else {
        if (adjacencyMatrix[piece.first][piece.second] != PieceColour::NONE) removeSuccessful = true;
        adjacencyMatrix[piece.first][piece.second] = PieceColour::NONE;
    }

    // Remove all pieces which are no longer connected to ground
    removeNotConnectedToGround(piece);
    // TODO: remove unreachable nodes by resizing matrix (for this we need node id translations)
    return removeSuccessful;
}

std::vector<Edge> AdjacencyMatrixPosition::getPieces(Player player) const {
    PieceColour colour;
    switch (player) {
        case Player::LEFT:
            colour = PieceColour::BLUE;
            break;
        case Player::RIGHT:
            colour = PieceColour::RED;
            break;
        default:
            throw(std::domain_error("Unknown player case"));
    }

    std::vector<Edge> pieces;
    for (size_t i = 0; i < adjacencyMatrix.size(); i++) {
        for (size_t j = 0; j < adjacencyMatrix[i].size(); j++) {
            if (adjacencyMatrix[i][j] == colour) {
                pieces.push_back(std::make_pair(i, j));
            }
        }
    }
    return pieces;
}

void AdjacencyMatrixPosition::removeNotConnectedToGround(Edge removedPiece) {
    std::vector<bool> visited(adjacencyMatrix.size(), false);
    std::queue<NodeId> queue;
    queue.push(groundId);
    bool firstVisited = false;
    bool secondVisited = false;
    while (!queue.empty()) {
        NodeId current = queue.front();
        queue.pop();
        visited[current] = true;
        firstVisited |= current == removedPiece.first;
        secondVisited |= current == removedPiece.second;
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

void AdjacencyMatrixPosition::removeNotConnectedToGround() {
    removeNotConnectedToGround(std::make_pair(-1, -1));
}

void AdjacencyMatrixPosition::printHumanReadable() const {
    std::cout << "===== AdjacencyMatrixPosition =====" << std::endl;
    for (auto row : adjacencyMatrix) {
        for (auto pieceColour : row) {
            if (pieceColour == PieceColour::BLUE) std::cout << "B";
            else if (pieceColour == PieceColour::RED) std::cout << "R";
            else if (pieceColour == PieceColour::NONE) std::cout << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "===================================" << std::endl;
}
