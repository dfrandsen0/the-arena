#ifndef MECHANICS_GAME_OBJECTS_WALL_H_
#define MECHANICS_GAME_OBJECTS_WALL_H_

#include "..\..\config.h"

class Wall {
public:
private:
    int health = MECHANICS_WALL_STARTING_HEALTH;
    int* delayedAttacks;
public:
    Wall();
    ~Wall();

    int GetHealth();
    void AddDelayedAttack();
    void ShiftAttacks();
private:
};

#endif