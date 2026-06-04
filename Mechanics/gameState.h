#ifndef MECHANICS_GAME_STATE_H_
#define MECHANICS_GAME_STATE_H_

#include <utility>

#include "GameStructure\worldState.h"
#include "GameObjects\player.h"
#include "GameObjects\unit.h"
#include "GameObjects\goal.h"
#include "GameObjects\tower.h"
#include "GameObjects\wall.h"
#include "GameStructure\bucket.h"
#include "..\Listener\mainInput.h"
#include "..\config.h"
#include "AI\ai.h"

class GameState {
public:

    //convert to private?
    int playersSize;
    int goalsSize;
    int unitsSize;
    int towersSize;
    int wallsSize;
private:
    WorldState* worldState = nullptr;

    Player** players = nullptr;
    Goal** goals = nullptr;

    Unit** units = nullptr;
    int lastUnit = 0;

    Bucket** buckets = nullptr;
    int bucketsSize = 0;

    // placed by tower id
    //   player 1's towers are 0 - max_towers - 1, then player 2, 3, 4
    Tower** towers = nullptr;

    Wall** walls = nullptr;

    //uses GameState, so initiated last
    AIModule* aiModule = nullptr;
public:
    GameState(int numPlayers, std::pair<int, int>* playersToSides);//, GraphicsModule* graphicsModule);
    ~GameState();

    void ClearBuckets();
    Bucket* GetBucket(int index);

    WorldState* GetWorldState();

    AIModule* GetAIModule();

    bool AtMaxTowers(int player);

    void FindRelativeXY(int clickX, int clickY, int* worldX, int* worldY);

    void GoalToUnitStart(int goalX, int goalY, int* unitX, int* unitY);

    WorldStateID GetWorldItem(int worldX, int worldY);
    Tower* FindTowerByXY(int worldX, int worldY);
    Tower** GetAllTowers();

    bool WithinSelectedRadius(Tower* tower, int worldX, int worldY);

    void IncrementPlayerMoney();
    int GetPlayerMoney(int playerNum);
    int GetMoneyDifference(int playerNum);
    void UpdateLastMoneys();

    Goal* GetGoal(int playerNum, int spawnNum);
    Player* GetPlayer(int playerNum);
    int GetLastUnit();
    Unit* GetUnit(int index);
    Tower* GetTower(int id);

    void HandleInput(MainInput* input, int playerNum);
    void UpdateTowers();
    void UpdateWalls();
    void UpdateUnits();
    bool UpdateGoals(int thisPlayer);

    void CalculateCollisions();

    Goal* CheckGoalOverlap(int worldX, int worldY);

    bool IsPlayerOut(int playerNum);

private:
    void InitPlayers(int numPlayers, std::pair<int, int>* playersToSides);
    void InitGoals(int numPlayers, std::pair<int, int>* playersToSides);
    void InitTowers(int numPlayers);
    void InitWalls();

    void PutUnitInBuckets(Unit* unit);

    void CreateUnit(int playerNum, int spawnValues, UnitType type);
    void CreateWall(int playerNum, int worldX, int worldY);
    void CreateTower(int playerNum, int worldX, int worldY);
    void RemoveTower(int playerNum, int towerId);
    void RemoveWall(int index);
    void RemoveUnit(int index);

    void ChangeTowerTarget(int towerId, int worldX, int worldY);

    void TowerAction(Tower* tower);
    void UpdateTowerHealth(Tower* tower);
    bool TowerTargetExists(Tower* tower);
    Unit* FindUnitWithinTower(Tower* tower);
    bool WithinTowerRange(int towerX, int towerY, int unitX, int unitY);
    void TowerShoot(Tower* tower, Unit* targetUnit);

    void FindTowerWorldCenter(int cellX, int cellY, int* worldX, int* worldY);
    int GetDistance(Unit* unit, UnitDirection priorityDirection);

    bool CheckCollide(Unit* unit1, Unit* unit2);
    void DamageUnits(Unit* unit1, Unit* unit2);
};

#endif