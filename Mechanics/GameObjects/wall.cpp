#include <cstring>
#include <iostream>

#include "wall.h"

Wall::Wall() {
    this->delayedAttacks = new int[MECHANICS_TOWER_FIRING_LENGTH + 1];
    memset(this->delayedAttacks, 0, sizeof(int) * (MECHANICS_TOWER_FIRING_LENGTH + 1));
}

Wall::~Wall() {
    
}

int Wall::GetHealth() {
    return this->health;
}

void Wall::AddDelayedAttack() {
    this->delayedAttacks[MECHANICS_TOWER_FIRING_LENGTH] += 1;
}

void Wall::ShiftAttacks() {
    this->health -= this->delayedAttacks[0];
    for(int i = 0; i < MECHANICS_TOWER_FIRING_LENGTH; i++) {
        this->delayedAttacks[i] = this->delayedAttacks[i + 1];
    }
    this->delayedAttacks[MECHANICS_TOWER_FIRING_LENGTH] = 0;
}