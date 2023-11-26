#ifndef POSITION_H
#define POSITION_H

#include <vector>
#include <cstddef>

typedef size_t NodeId;
typedef std::pair<NodeId, NodeId> Edge;

enum class PieceColour {
	BLUE = 1,
	RED = -1,
	NONE = 0,
};

class ClassicalPosition {
public:
    virtual ClassicalPosition *clone() const = 0;
    virtual size_t getNodeCount() const = 0;
    virtual void increaseNodeCount(size_t count) = 0;
    virtual void addPiece(NodeId from, NodeId to, PieceColour color) = 0;
    virtual void removePiece(NodeId from, NodeId to) = 0;
    virtual std::vector<Edge> getBluePieces() const = 0;
    virtual std::vector<Edge> getRedPieces() const = 0;

    virtual ~ClassicalPosition() = default;
};

class Position {
public:
    Position(const std::vector<const ClassicalPosition*>& realisations);
    Position(const ClassicalPosition *classicalPosition);
    size_t getWidth() const;
    bool empty() const;
    void addRealisation(const ClassicalPosition *realization);
    const ClassicalPosition& getRealisation(size_t index) const;
    // Gets the blue pieces across all realisations.
    std::vector<Edge> getBluePieces() const;
    // Gets the red pieces across all realisations.
    std::vector<Edge> getRedPieces() const;

    ~Position();
    
private:
    std::vector<const ClassicalPosition*> realisations;
};

// Creates a 1-wide starting position for the restricted variant of Hackenbush.
const Position& createRestrictedPosition(size_t nodeCount, size_t nBlueHalfs, size_t nRedHalfs, size_t nBlueWholes, size_t nRedWholes);

#endif // POSITION_H
