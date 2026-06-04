#include <cstring>
#include <iostream>

#include "tower.h"
#include "wall.h"
#include "..\..\config.h"

Tower::Tower(PlayerColor color, int towerId, int worldX, int worldY, UnitDirection priorityDirection) {
    this->color = color;
    this->towerId = towerId;
    this->worldX = worldX;
    this->worldY = worldY;

    this->screenCenterX = worldX * GRAPHICS_UI_MAIN_ARENA_CELL_SIZE + GRAPHICS_UI_MAIN_ARENA_HALF_CELL_SIZE + GRAPHICS_UI_MAIN_ARENA_INSET_X - 1;
    this->screenCenterY = worldY * GRAPHICS_UI_MAIN_ARENA_CELL_SIZE + GRAPHICS_UI_MAIN_ARENA_HALF_CELL_SIZE + GRAPHICS_UI_MAIN_ARENA_INSET_Y - 1;

    this->priorityDirection = priorityDirection;

    this->delayedAttacks = new int[MECHANICS_TOWER_FIRING_LENGTH + 1];
    memset(this->delayedAttacks, 0, sizeof(int) * (MECHANICS_TOWER_FIRING_LENGTH + 1));

    switch(color) {
        case PlayerColor::BLUE:
            this->worldStateId = WorldStateID::BLUE_TOWER;
            break;
        case PlayerColor::RED:
            this->worldStateId = WorldStateID::RED_TOWER;
            break;
        case PlayerColor::GREEN:
            this->worldStateId = WorldStateID::GREEN_TOWER;
            break;
        case PlayerColor::YELLOW:
            this->worldStateId = WorldStateID::YELLOW_TOWER;
            break;
    }
}

Tower::~Tower() {
    
}

PlayerColor Tower::GetColor() {
    return this->color;
}

int Tower::GetTowerID() {
    return this->towerId;
}

int Tower::GetX() {
    return this->worldX;
}

int Tower::GetY() {
    return this->worldY;
}

int Tower::GetScreenX() {
    return this->screenCenterX;
}

int Tower::GetScreenY() {
    return this->screenCenterY;
}

void Tower::DecrementHealth() {
    (this->health)--;
}

void Tower::ShiftDelayedAttacks() {
    this->health -= this->delayedAttacks[0];
    
    for(int i = 0; i < MECHANICS_TOWER_FIRING_LENGTH; i++) {
        this->delayedAttacks[i] = this->delayedAttacks[i + 1];
    }

    this->delayedAttacks[MECHANICS_TOWER_FIRING_LENGTH] = 0;
}

int Tower::GetHealth() {
    return this->health;
}

WorldStateID Tower::GetWorldStateId() {
    return this->worldStateId;
}

bool Tower::IsWaiting() {
    return this->waiting;
}

void Tower::SetWaiting(bool newWaiting) {
    this->waiting = newWaiting;
}

int Tower::GetDelay() {
    return this->delay;
}

void Tower::SetDelay(int delay) {
    this->delay = delay;
}

void Tower::DecrementDelay() {
    (this->delay)--;
}

UnitDirection Tower::GetPriorityDirection() {
    return this->priorityDirection;
}

bool Tower::WithinPriorityDirection(UnitDirection unitTrajectory) {
    if(unitTrajectory == this->priorityDirection) {
        return true;
    }

    switch(this->priorityDirection) {
        case UnitDirection::DOWN:
            if(unitTrajectory == UnitDirection::DOWN_RIGHT ||
               unitTrajectory == UnitDirection::DOWN_LEFT) {
                return true;
            }
            break;
        case UnitDirection::UP:
            if(unitTrajectory == UnitDirection::UP_RIGHT ||
               unitTrajectory == UnitDirection::UP_LEFT) {
                return true;
            }
            break;
        case UnitDirection::RIGHT:
            if(unitTrajectory == UnitDirection::DOWN_RIGHT ||
               unitTrajectory == UnitDirection::UP_RIGHT) {
                return true;
            }
            break;
        default:    //left
            if(unitTrajectory == UnitDirection::DOWN_LEFT ||
               unitTrajectory == UnitDirection::UP_LEFT) {
                return true;
            }
            break;
    }
    
    return false;
}

bool Tower::IsTargeting() {
    return this->targeting;
}

void Tower::SetTargeting(bool newTargeting) {
    this->targeting = newTargeting;
}

int Tower::GetTargetX() {
    return this->targetX;
}

void Tower::SetTargetX(int newX) {
    this->targetX = newX;
}

int Tower::GetTargetY() {
    return this->targetY;
}

void Tower::SetTargetY(int newY) {
    this->targetY = newY;
}

TowerTargetType Tower::GetTargetType() {
    return this->targetType;
}

void Tower::SetTargetType(TowerTargetType type) {
    this->targetType = type;
}

Wall* Tower::GetTargetWall() {
    return this->targetWall;
}

void Tower::SetTargetWall(Wall* wall) {
    this->targetWall = wall;
}

Tower* Tower::GetTargetTower() {
    return this->targetTower;
}

void Tower::SetTargetTower(Tower* tower) {
    this->targetTower = tower;
}

void Tower::AddDelayedAttack() {
    delayedAttacks[MECHANICS_TOWER_FIRING_LENGTH] += 1;
}
