#include "unit.h"
#include "..\..\Graphics\BitmapInfo\bitmapObj.h"
#include "..\..\config.h"

Unit::Unit(UnitType type, PlayerColor color, int x, int y) {
    this->type = type;
    this->color = color;
    this->x = x;
    this->y = y;
    this->health = MECHANICS_UNIT_STARTING_HEALTH;
}

Unit::~Unit() {
    
}

UnitType Unit::GetType() {
    return this->type;
}

PlayerColor Unit::GetColor() {
    return this->color;
}

int Unit::GetX() {
    return this->x;
}

void Unit::SetX(int newX) {
    this->x = newX;
}

int Unit::GetY() {
    return this->y;
}

void Unit::SetY(int newY) {
    this->y = newY;
}

int Unit::GetPixelX() {
    return (this->x / MECHANICS_UNIT_SPACE_AMPLIFIER) + this->offsetX;
}

int Unit::GetPixelY() {
    return (this->y / MECHANICS_UNIT_SPACE_AMPLIFIER) + this->offsetY;
}

int Unit::GetHealth() {
    return this->health;
}

void Unit::TakeDamage(int damage) {
    this->health -= damage;
}

UnitDirection Unit::GetTrajectory() {
    return this->trajectory;
}

void Unit::SetTrajectory(UnitDirection direction) {
    this->trajectory = direction;
}

void Unit::AddDeathSentence() {
    if(this->deathDelay < 0) {
        this->deathDelay = MECHANICS_TOWER_FIRING_LENGTH + 1;
    }
}

int Unit::GetDeathDelay() {
    return this->deathDelay;
}

void Unit::DecrementDeathDelay() {
    (this->deathDelay)--;
}