#ifndef MECHANICS_GAME_STRUCTURE_WORLD_UPDATE_H_
#define MECHANICS_GAME_STRUCTURE_WORLD_UPDATE_H_

#include "..\..\config.h"

struct WorldUpdate {
public:
    int worldX;
    int worldY;
    WorldStateID stateId;

    WorldUpdate(int worldX, int worldY, WorldStateID stateId);
    ~WorldUpdate();
};

#endif