#include "goal.h"
#include "..\..\config.h"

Goal::Goal(int playerNum, int x, int y, int spawnNum) {
    this->playerNum = playerNum;
    this->health = MECHANICS_GOAL_STARTING_HEALTH;
    this->x = x;
    this->y = y;
    this->spawnNum = spawnNum;
}

Goal::~Goal() {
    
}

int Goal::GetPlayerNum() {
    return this->playerNum;
}

int Goal::GetHealth() {
    return this->health;
}

void Goal::DecrementHealth() {
    (this->health)--;
}

int Goal::GetX() {
    return this->x;
}

int Goal::GetY() {
    return this->y;
}

int Goal::GetSpawnNum() {
    return this->spawnNum;
}