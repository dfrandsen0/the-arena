#ifndef MECHANICS_AI_PATHFINDER_H_
#define MECHANICS_AI_PATHFINDER_H_

#include <utility>

#include "..\GameObjects\goal.h"
#include "..\..\config.h"

class PathFinder {
public:
protected:
    //shared from ai.h
    std::pair<int, int>* criticalCells = nullptr;
    int criticalCellsSize = 0;

public:
    explicit PathFinder(std::pair<int, int>* criticalCells, int criticalCellsSize);
    virtual ~PathFinder();

    virtual void InitPathFinder(int numPlayers, std::pair<int, int>* playersToSides) = 0;
    virtual UnitDirection GetDirection(int playerNum, int worldX, int worldY, int cellX, int cellY) = 0;

    virtual void WorldUpdate(AIUpdateType type, int playerNum, int cellX, int cellY) = 0;
    virtual void UpdatePathFinding() = 0;

    virtual void RemoveGoal(Goal* goal) = 0;
private:
};

#endif