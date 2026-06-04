#include <iostream>
#include <utility>
#include <vector>

#include "worldState.h"
#include "worldUpdate.h"
#include "..\..\Graphics\graphics.h"
#include "..\..\config.h"

WorldState::WorldState(int numPlayers, std::pair<int, int>* playersToSides) {
    this->world = new WorldStateID[MECHANICS_WORLD_STATE_WIDTH * MECHANICS_WORLD_STATE_HEIGHT];
    this->InitWorld(numPlayers, playersToSides);

    this->worldUpdates = new WorldUpdate*[GRAPHICS_MAX_WORLD_UPDATES];
    this->worldUpdateCount = 0;
}

WorldState::~WorldState() {
    delete[] this->world;

    for(int i = 0; i < this->worldUpdateCount; i++) {
        delete this->worldUpdates[i];
    }
    this->worldUpdateCount = 0;

    delete[] this->worldUpdates;
}

WorldUpdate** WorldState::GetWorldUpdates() {
    return this->worldUpdates;
}

int WorldState::GetWorldUpdateCount() {
    return this->worldUpdateCount;
}

void WorldState::ResetWorldUpdateCount() {
    this->worldUpdateCount = 0;
}

void WorldState::InitWorld(int numPlayers, std::pair<int, int>* playersToSides) {
    //Zero out everything, puts empty_both everywhere
    for(int y = 0; y < MECHANICS_WORLD_STATE_WIDTH; y++) {
        for(int x = 0; x < MECHANICS_WORLD_STATE_HEIGHT; x++) {
            this->world[(y * MECHANICS_WORLD_STATE_WIDTH) + x] = WorldStateID::EMPTY_BOTH;
        }
    }

    //edge corners
    this->world[0] = WorldStateID::EDGE_CORNER_TOP_LEFT;
    this->world[24] = WorldStateID::EDGE_CORNER_TOP_RIGHT;
    this->world[24 * MECHANICS_WORLD_STATE_WIDTH] = WorldStateID::EDGE_CORNER_BOTTOM_LEFT;
    this->world[(MECHANICS_WORLD_STATE_HEIGHT * MECHANICS_WORLD_STATE_WIDTH) - 1] = WorldStateID::EDGE_CORNER_BOTTOM_RIGHT;

    //horizontal edges with bottom empty row (dotted lines right side)
    int bottomRow = MECHANICS_WORLD_STATE_WIDTH * (MECHANICS_WORLD_STATE_HEIGHT - 1);
    int secondBottomRow = MECHANICS_WORLD_STATE_WIDTH * (MECHANICS_WORLD_STATE_HEIGHT - 2);
    for(int i = 1; i < MECHANICS_WORLD_STATE_WIDTH - 1; i++) {
        this->world[i] = WorldStateID::EDGE_HORIZONTAL;
        this->world[bottomRow + i] = WorldStateID::EDGE_HORIZONTAL;
        this->world[secondBottomRow + i] = WorldStateID::EMPTY_RIGHT;
    }

    //vertical edges with right empty column (dotted lines bottom side)
    for(int i = 1; i < MECHANICS_WORLD_STATE_HEIGHT - 1; i++) {
        this->world[i * MECHANICS_WORLD_STATE_WIDTH] = WorldStateID::EDGE_VERTICAL;
        this->world[(i + 1) * MECHANICS_WORLD_STATE_WIDTH - 1] = WorldStateID::EDGE_VERTICAL;
        this->world[(i + 1) * MECHANICS_WORLD_STATE_WIDTH - 2] = WorldStateID::EMPTY_BOTTOM;
    }

    //single empty_neither tile
    this->world[(MECHANICS_WORLD_STATE_WIDTH - 1)* MECHANICS_WORLD_STATE_HEIGHT - 2] = WorldStateID::EMPTY_NEITHER;

    //goals
    this->PlaceGoalsInWorld(numPlayers, playersToSides);
}

WorldStateID* WorldState::GetWorld() {
    return this->world;
}

WorldStateID WorldState::GetItem(int x, int y) {
    return this->world[y * MECHANICS_WORLD_STATE_WIDTH + x];
}

void WorldState::PutItem(int x, int y, WorldStateID id) {
    this->world[y * MECHANICS_WORLD_STATE_WIDTH + x] = id;

    this->worldUpdates[this->worldUpdateCount] = new WorldUpdate(x, y, id);
    (this->worldUpdateCount)++;
}

void WorldState::RemoveItem(int x, int y) {
    int index = y * MECHANICS_WORLD_STATE_WIDTH + x;
    WorldStateID id;

    switch(this->world[index]) {
        case WorldStateID::BLUE_TOWER:
        case WorldStateID::RED_TOWER:
        case WorldStateID::GREEN_TOWER:
        case WorldStateID::YELLOW_TOWER:
        case WorldStateID::WALL:
            if(x == MECHANICS_WORLD_STATE_WIDTH - 2 &&
               y == MECHANICS_WORLD_STATE_HEIGHT - 2) {
                id = WorldStateID::EMPTY_NEITHER;
            } else if(x == MECHANICS_WORLD_STATE_WIDTH - 2) {
                id = WorldStateID::EMPTY_BOTTOM;
            } else if(y == MECHANICS_WORLD_STATE_HEIGHT - 2) {
                id = WorldStateID::EMPTY_RIGHT;
            } else {
                id = WorldStateID::EMPTY_BOTH;
            }
            
            break;
        default:
            return;
    }

    this->world[index] = id;
    this->worldUpdates[worldUpdateCount] = new WorldUpdate(x, y, id);
    (this->worldUpdateCount)++;
}

bool WorldState::IsEmpty(int x, int y) {
    WorldStateID id = this->GetItem(x, y);
    if(id == WorldStateID::EMPTY_BOTH ||
       id == WorldStateID::EMPTY_BOTTOM ||
       id == WorldStateID::EMPTY_RIGHT ||
       id == WorldStateID::EMPTY_NEITHER) {
        return true;
    }
    return false;
}

void WorldState::PlaceGoalsInWorld(int numPlayers, std::pair<int, int>* playersToSides) {
    std::vector<std::pair<int, int>> currSide;
    WorldStateID currPlayerGoal;
    int offset;
    WorldStateID offsetNegId;
    WorldStateID offsetPosId;
    int goalTarget;

    for(int i = 0; i < numPlayers; i++) {
        currSide = MECHANICS_WORLD_STATE_GOALS_XYS[playersToSides[i].second];
        currPlayerGoal = this->PlayerToGoalID(playersToSides[i].first);
        
        if((playersToSides[i].second == 0) || (playersToSides[i].second == 2)) {
            offset = 1;
            offsetNegId = WorldStateID::EDGE_GOAL_LEFT;
            offsetPosId = WorldStateID::EDGE_GOAL_RIGHT;
        } else {
            offset = MECHANICS_WORLD_STATE_WIDTH;
            offsetNegId = WorldStateID::EDGE_GOAL_BOTTOM;
            offsetPosId = WorldStateID::EDGE_GOAL_TOP;
        }

        for(int j = 0; j < MECHANICS_NUM_GOALS; j++) {
            goalTarget = currSide[j].second * MECHANICS_WORLD_STATE_WIDTH + currSide[j].first;
            this->world[goalTarget] = currPlayerGoal;
            this->world[goalTarget - offset] = offsetNegId;
            this->world[goalTarget + offset] = offsetPosId;
        }
    }
}

WorldStateID WorldState::PlayerToGoalID(int playerNum) {
    switch(playerNum) {
        case 1:
            return WorldStateID::GOAL_BLUE;
        case 2:
            return WorldStateID::GOAL_RED;
        case 3:
            return WorldStateID::GOAL_GREEN;
        default:
            return WorldStateID::GOAL_YELLOW;
            break;
    }
}
