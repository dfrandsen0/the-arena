#ifndef MECHANICS_AI_H_
#define MECHANICS_AI_H_

#include <utility>
#include <bitset>
#include <stack>

#include "pathFinder.h"
#include "..\GameObjects\unit.h"
#include "..\GameObjects\goal.h"
#include "..\GameStructure\bucket.h"
#include "..\..\config.h"

class GameState;

class AIModule {
public:
private:
    GameState* gameState = nullptr;

    PathFinder* pathfinder = nullptr;

    //Checks fill and DFS
    std::pair<int, int>* criticalCells = nullptr;
    int criticalCellsSize;

    int fillWidth;
    int fillHeight;
    std::bitset<(MECHANICS_WORLD_STATE_WIDTH - 2) * (MECHANICS_WORLD_STATE_HEIGHT - 2)> worldBits;
    std::bitset<(MECHANICS_WORLD_STATE_WIDTH - 2) * (MECHANICS_WORLD_STATE_HEIGHT - 2)> criticalCellsBits;
    std::bitset<(MECHANICS_WORLD_STATE_WIDTH - 2) * (MECHANICS_WORLD_STATE_HEIGHT - 2)> filledVisited;
    std::stack<int> filledStack;
    int startBit;

public:
    AIModule(GameState* gameState, int numPlayers, std::pair<int, int>* playersToSides);
    ~AIModule();

    PathFinder* GetPathFinder();

    bool IsFillable(int worldX, int worldY);
    bool FillableScanBucket(Bucket* bucket, int cellX, int cellY);

    void UpdateAIWorld(AIUpdateType type, int playerNum, int worldX, int worldY);
    void RemoveGoal(Goal* goal);

    UnitDirection GetUnitDirection(Unit* currUnit, bool* withinGoal);

    void CellToWorld(int cellX, int cellY, int* worldX, int* worldY);
    bool CheckUnitTouchWorldCell(int unitX, int unitY, int worldX, int worldY);
private:
    bool CheckGoalCollision(Unit* unit, int cellX, int cellY, int worldX, int worldY);

    void AddWorldBit(int worldX, int worldY);
    void RemoveWorldBit(int worldX, int worldY);
    int WorldToBit(int worldX, int worldY);

    void UnitToWorld(int unitX, int unitY, int* worldX, int* worldY);
    void WorldToCell(int worldX, int worldY, int* cellX, int* cellY);
};

#endif