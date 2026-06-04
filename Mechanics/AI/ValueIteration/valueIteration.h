#ifndef MECHANICS_AI_VALUE_ITERATION_H_
#define MECHANICS_AI_VALUE_ITERATION_H_

#include <utility>
#include <queue>
#include <stack>
#include <bitset>

#include "..\pathFinder.h"
#include "valuesObj.h"
#include "..\..\GameObjects\goal.h"
#include "..\..\..\config.h"

class ValueIteration : public PathFinder {
public:
private:
    ValueObj** valueSets = nullptr;
    int valueSetsSize = 0;

    int* straightDirections = nullptr;
    int* diagonalDirections = nullptr;

    std::queue<int> cellUpdateQueue;
    std::queue<int> helperQueue;
    std::queue<int> originalCells;
    std::stack<int> importanceStack;
    std::bitset<MECHANICS_WORLD_STATE_WIDTH * MECHANICS_WORLD_STATE_HEIGHT> visited;

    std::bitset<MECHANICS_WORLD_STATE_WIDTH * MECHANICS_WORLD_STATE_HEIGHT> importantCells;
    std::bitset<MECHANICS_WORLD_STATE_WIDTH * MECHANICS_WORLD_STATE_HEIGHT> unimportantCells;
public:
    explicit ValueIteration(std::pair<int, int>* criticalCells, int criticalCellsSize);
    ~ValueIteration();

    void InitPathFinder(int numPlayers, std::pair<int, int>* playersToSides) override;
    UnitDirection GetDirection(int playerNum, int worldX, int worldY, int cellX, int cellY) override;

    void WorldUpdate(AIUpdateType type, int playerNum, int cellX, int cellY) override;
    void UpdatePathFinding() override;
    void RemoveGoal(Goal* goal) override;

    void InitValueSet(int playerNum, int numPlayers, std::pair<int, int>* playersToSides);
private:
    ValueObj* FindValueSet(int playerNum);

    void SetWorldBit(int index);
    void ResetWorldBit(int index);

    void ChangeGoalReward(int playerNum, int index);
    void ChangeTowerReward(AIUpdateType type, int playerNum, int cellX, int cellY);

    bool CheckImportance(int index);

    bool UpdateCell(ValueObj* valueSet, int index);
    void AddAffectedCells(ValueObj* valueSet, int index);

    //Should combine these two functions.
    UnitDirection GetRawDirection(ValueObj* valueSet, int index);
    int CalcCellValue(ValueObj* valueSet, int index);

    int CellToIndex(int cellX, int cellY);

};

#endif