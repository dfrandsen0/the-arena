#include <cstring>
#include <iostream>

#include "valuesObj.h"
#include "..\..\..\config.h"

ValueObj::ValueObj(int playerNum) {
    this->playerNum = playerNum;
    int numCells = MECHANICS_WORLD_STATE_WIDTH * MECHANICS_WORLD_STATE_HEIGHT;

    this->values = new int[numCells];
    memset(this->values, 0, sizeof(int) * numCells);

    this->rewards = new int[numCells];
    memset(this->rewards, 0, sizeof(int) * numCells);

    this->worldBits.reset();
    this->criticalBits.reset();
}

ValueObj::~ValueObj() {

}

void ValueObj::Print() {
    std::cout << "Player " << this->playerNum << std::endl;
    for(int i = 0; i < MECHANICS_WORLD_STATE_HEIGHT; i++) {
        for(int j = 0; j < MECHANICS_WORLD_STATE_WIDTH; j++) {
            std::cout << this->values[i * MECHANICS_WORLD_STATE_WIDTH + j] << " ";
        }
        std::cout << std::endl;
    }
}

void ValueObj::PrintBitSet() {
    std::cout << "Player " << this->playerNum << std::endl;

    for(int i = 0; i < MECHANICS_WORLD_STATE_WIDTH * MECHANICS_WORLD_STATE_HEIGHT; i++) {
        if(this->worldBits.test(i)) {
            std::cout << "1 ";
        } else {
            std::cout << "0 ";
        }

        if(i % MECHANICS_WORLD_STATE_WIDTH == MECHANICS_WORLD_STATE_WIDTH - 1) {
            std::cout << std::endl;
        }
    }
}