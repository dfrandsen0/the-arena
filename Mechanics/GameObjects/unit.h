#ifndef MECHANICS_GAME_OBJECTS_UNIT_H_
#define MECHANICS_GAME_OBJECTS_UNIT_H_

#include "..\..\config.h"

class Unit {
private:
    UnitType type;
    PlayerColor color;
    int health;
    int x; // relative to arena, * amplifier
    int y;
    int offsetX = GRAPHICS_UI_MAIN_ARENA_INSET_X - GRAPHICS_BITMAPS_UNIT_OFFSET;
    int offsetY = GRAPHICS_UI_MAIN_ARENA_INSET_Y - GRAPHICS_BITMAPS_UNIT_OFFSET;

    UnitDirection trajectory = UnitDirection::UP;

    int deathDelay = -1;
public:
    Unit(UnitType type, PlayerColor color, int x, int y);
    ~Unit();

    UnitType GetType();
    PlayerColor GetColor();

    int GetX();
    void SetX(int newX);

    int GetY();
    void SetY(int newY);

    int GetPixelX();
    int GetPixelY();

    int GetHealth();
    void TakeDamage(int damage);

    UnitDirection GetTrajectory();
    void SetTrajectory(UnitDirection direction);

    void AddDeathSentence();
    int GetDeathDelay();
    void DecrementDeathDelay();

private:
};

#endif