#ifndef MECHANICS_USER_STATE_H_
#define MECHANICS_USER_STATE_H_

#include "GameObjects\tower.h"
#include "..\config.h"

class UserState {
public:
private:
    UserSelect selected = UserSelect::NONE;
    Tower* selectedTower = nullptr;
    int delay = 0;

    int thisPlayer = 0;
    bool isAlive = true;

    WorldStateID personalGoalState;
    WorldStateID personalTowerState;
    BitmapID personalTowerBitmapId;
    BitmapID personalGoalBitmapId;
    BitmapID personalGoalSelectedId;

    BitmapID* buttonBitmapIds;

    bool* spawnsAlive = nullptr;
    bool* spawnsActive = nullptr;

    bool spawnsChanged = false;
public:
    UserState(int thisPlayer);
    ~UserState();

    UserSelect GetSelected();
    void SetSelected(UserSelect selection);

    Tower* GetSelectedTower();
    void SetSelectedTower(Tower* newTower);

    void SetDelay();
    void DecrementDelay();
    int GetDelay();

    int GetThisPlayer();
    bool ThisPlayerIsAlive();
    void KillThisPlayer();

    WorldStateID GetPersonalGoalState();
    WorldStateID GetPersonalTowerState();
    BitmapID GetPersonalTowerBitmapId();
    BitmapID GetPersonalGoalBitmapId();
    BitmapID GetPersonalGoalSelectedId();

    BitmapID GetButtonId(UserStateButtonID button);

    bool GetSpawn(int spawnNum);
    int GetSpawnValues();

    bool HaveSpawnsChanged();
    void SetSpawnsChanged(bool newValue);

    void ClickSpawn(int worldX, int worldY);
private:
    void ToggleSpawn(int spawnNum);

    int CoordsToSpawn(int worldX, int worldY);
};

#endif