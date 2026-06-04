#ifndef MECHANICS_GAME_OBJECTS_TOWER_H_
#define MECHANICS_GAME_OBJECTS_TOWER_H_

#include "wall.h"
#include "..\..\config.h"

class Tower {
private:
    PlayerColor color;
    int towerId;
    int worldX = 0;
    int worldY = 0;

    int screenCenterX = 0;
    int screenCenterY = 0;

    int health = MECHANICS_TOWER_STARTING_HEALTH;

    WorldStateID worldStateId;
    
    bool waiting = true;
    int delay = MECHANICS_TOWER_START_DELAY;

    UnitDirection priorityDirection = UnitDirection::UP;

    bool targeting = false;
    int targetX = -1;
    int targetY = -1;

    TowerTargetType targetType = TowerTargetType::WALL;
    Wall* targetWall = nullptr;
    Tower* targetTower = nullptr;

    int* delayedAttacks = nullptr;

public:
    Tower(PlayerColor color, int towerId, int worldX, int worldY, UnitDirection priorityDirection);
    ~Tower();

    PlayerColor GetColor();

    int GetTowerID();

    int GetX();
    int GetY();

    int GetScreenX();
    int GetScreenY();

    void DecrementHealth();
    void ShiftDelayedAttacks();
    int GetHealth();
    WorldStateID GetWorldStateId();

    bool IsWaiting();
    void SetWaiting(bool newWaiting);

    int GetDelay();
    void SetDelay(int delay);
    void DecrementDelay();

    UnitDirection GetPriorityDirection();
    bool WithinPriorityDirection(UnitDirection unitTrajectory);

    bool IsTargeting();
    void SetTargeting(bool newTargeting);

    int GetTargetX();
    void SetTargetX(int newX);

    int GetTargetY();
    void SetTargetY(int newY);

    TowerTargetType GetTargetType();
    void SetTargetType(TowerTargetType type);

    Wall* GetTargetWall();
    void SetTargetWall(Wall* wall);
    Tower* GetTargetTower();
    void SetTargetTower(Tower* tower);

    void AddDelayedAttack();
};

#endif