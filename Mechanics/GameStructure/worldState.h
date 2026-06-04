#ifndef MECHANICS_GAME_STRUCTURE_WORLD_STATE_H_
#define MECHANICS_GAME_STRUCTURE_WORLD_STATE_H_

#include <utility>

#include "worldUpdate.h"
#include "..\..\config.h"

class WorldState {
public:
private:
    WorldStateID* world = nullptr;
    WorldUpdate** worldUpdates = nullptr;
    int worldUpdateCount = 0;
public:
    WorldState(int numPlayers, std::pair<int, int>* playersToSides);
    ~WorldState();

    WorldUpdate** GetWorldUpdates();
    int GetWorldUpdateCount();
    void ResetWorldUpdateCount();

    void InitWorld(int numPlayers, std::pair<int, int>* playersToSides);
    WorldStateID* GetWorld();

    WorldStateID GetItem(int x, int y);
    void PutItem(int x, int y, WorldStateID id);
    void RemoveItem(int x, int y);
    bool IsEmpty(int x, int y);
private:
    void PlaceGoalsInWorld(int numPlayers, std::pair<int, int>* playersToSides);
    WorldStateID PlayerToGoalID(int playerNum);
};

#endif