#ifndef MECHANICS_AI_VALUE_ITERATION_VALUES_OBJ_H_
#define MECHANICS_AI_VALUE_ITERATION_VALUES_OBJ_H_

#include <bitset>

#include "..\..\..\config.h"

struct ValueObj {
public:
    ValueObj(int playerNum);
    ~ValueObj();

    int playerNum;
    int* values;
    int* rewards;
    std::bitset<MECHANICS_WORLD_STATE_WIDTH * MECHANICS_WORLD_STATE_HEIGHT> worldBits;
    std::bitset<MECHANICS_WORLD_STATE_WIDTH * MECHANICS_WORLD_STATE_HEIGHT> criticalBits;

    void Print();
    void PrintBitSet();
};

#endif