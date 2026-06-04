#include <iostream>

#include "userState.h"
#include "GameObjects\tower.h"
#include "..\config.h"

UserState::UserState(int thisPlayer) {
    this->thisPlayer = thisPlayer;
    this->buttonBitmapIds = new BitmapID[8];

    this->spawnsAlive = new bool[MECHANICS_NUM_GOALS];
    this->spawnsActive = new bool[MECHANICS_NUM_GOALS];

    for(int i = 0; i < MECHANICS_NUM_GOALS; i++) {
        this->spawnsAlive[i] = true;
        this->spawnsActive[i] = false;
    }

    this->spawnsActive[1] = true;

    switch(this->thisPlayer) {
        case 1:
            this->personalGoalState = WorldStateID::GOAL_BLUE;
            this->personalTowerState = WorldStateID::BLUE_TOWER;
            this->personalTowerBitmapId = BitmapID::TOWER_BLUE;
            this->personalGoalBitmapId = BitmapID::BLUE_GOAL;
            this->personalGoalSelectedId = BitmapID::BLUE_GOAL_SELECTED;
            this->buttonBitmapIds[0] = BitmapID::BLUE_TOWER_BUTTON;
            this->buttonBitmapIds[1] = BitmapID::BLUE_SQUARE_BUTTON;
            this->buttonBitmapIds[2] = BitmapID::BLUE_TRIANGLE_BUTTON;
            this->buttonBitmapIds[3] = BitmapID::BLUE_CIRCLE_BUTTON;
            this->buttonBitmapIds[4] = BitmapID::BLUE_TOWER_PRESSED;
            this->buttonBitmapIds[5] = BitmapID::BLUE_SQUARE_PRESSED;
            this->buttonBitmapIds[6] = BitmapID::BLUE_TRIANGLE_PRESSED;
            this->buttonBitmapIds[7] = BitmapID::BLUE_CIRCLE_PRESSED;
            break;
        case 2:
            this->personalGoalState = WorldStateID::GOAL_RED;
            this->personalTowerState = WorldStateID::RED_TOWER;
            this->personalTowerBitmapId = BitmapID::TOWER_RED;
            this->personalGoalBitmapId = BitmapID::RED_GOAL;
            this->personalGoalSelectedId = BitmapID::RED_GOAL_SELECTED;
            this->buttonBitmapIds[0] = BitmapID::RED_TOWER_BUTTON;
            this->buttonBitmapIds[1] = BitmapID::RED_SQUARE_BUTTON;
            this->buttonBitmapIds[2] = BitmapID::RED_TRIANGLE_BUTTON;
            this->buttonBitmapIds[3] = BitmapID::RED_CIRCLE_BUTTON;
            this->buttonBitmapIds[4] = BitmapID::RED_TOWER_PRESSED;
            this->buttonBitmapIds[5] = BitmapID::RED_SQUARE_PRESSED;
            this->buttonBitmapIds[6] = BitmapID::RED_TRIANGLE_PRESSED;
            this->buttonBitmapIds[7] = BitmapID::RED_CIRCLE_PRESSED;
            break;
        case 3:
            this->personalGoalState = WorldStateID::GOAL_GREEN;
            this->personalTowerState = WorldStateID::GREEN_TOWER;
            this->personalTowerBitmapId = BitmapID::TOWER_GREEN;
            this->personalGoalBitmapId = BitmapID::GREEN_GOAL;
            this->personalGoalSelectedId = BitmapID::GREEN_GOAL_SELECTED;
            this->buttonBitmapIds[0] = BitmapID::GREEN_TOWER_BUTTON;
            this->buttonBitmapIds[1] = BitmapID::GREEN_SQUARE_BUTTON;
            this->buttonBitmapIds[2] = BitmapID::GREEN_TRIANGLE_BUTTON;
            this->buttonBitmapIds[3] = BitmapID::GREEN_CIRCLE_BUTTON;
            this->buttonBitmapIds[4] = BitmapID::GREEN_TOWER_PRESSED;
            this->buttonBitmapIds[5] = BitmapID::GREEN_SQUARE_PRESSED;
            this->buttonBitmapIds[6] = BitmapID::GREEN_TRIANGLE_PRESSED;
            this->buttonBitmapIds[7] = BitmapID::GREEN_CIRCLE_PRESSED;
            break;
        default:
            this->personalGoalState = WorldStateID::GOAL_YELLOW;
            this->personalTowerState = WorldStateID::YELLOW_TOWER;
            this->personalTowerBitmapId = BitmapID::TOWER_YELLOW;
            this->personalGoalBitmapId = BitmapID::YELLOW_GOAL;
            this->personalGoalSelectedId = BitmapID::YELLOW_GOAL_SELECTED;
            this->buttonBitmapIds[0] = BitmapID::YELLOW_TOWER_BUTTON;
            this->buttonBitmapIds[1] = BitmapID::YELLOW_SQUARE_BUTTON;
            this->buttonBitmapIds[2] = BitmapID::YELLOW_TRIANGLE_BUTTON;
            this->buttonBitmapIds[3] = BitmapID::YELLOW_CIRCLE_BUTTON;
            this->buttonBitmapIds[4] = BitmapID::YELLOW_TOWER_PRESSED;
            this->buttonBitmapIds[5] = BitmapID::YELLOW_SQUARE_PRESSED;
            this->buttonBitmapIds[6] = BitmapID::YELLOW_TRIANGLE_PRESSED;
            this->buttonBitmapIds[7] = BitmapID::YELLOW_CIRCLE_PRESSED;
            break;
    }
}

UserState::~UserState() {
    delete[] this->buttonBitmapIds;
    delete[] this->spawnsActive;
    delete[] this->spawnsAlive;
}

UserSelect UserState::GetSelected() {
    return this->selected;
}

void UserState::SetSelected(UserSelect selection) {
    this->selected = selection;
}

Tower* UserState::GetSelectedTower() {
    return this->selectedTower;
}

void UserState::SetSelectedTower(Tower* newTower) {
    this->selectedTower = newTower;
}

void UserState::SetDelay() {
    this->delay = NETWORKING_FRAME_DELAY;
}

void UserState::DecrementDelay() {
    if(this->delay > 0) {
        (this->delay)--;
    }
}

int UserState::GetDelay() {
    return this->delay;
}

int UserState::GetThisPlayer() {
    return this->thisPlayer;
}

bool UserState::ThisPlayerIsAlive() {
    return this->isAlive;
}

void UserState::KillThisPlayer() {
    this->isAlive = false;
    this->selected = UserSelect::NONE;
}

WorldStateID UserState::GetPersonalGoalState() {
    return this->personalGoalState;
}

WorldStateID UserState::GetPersonalTowerState() {
    return this->personalTowerState;
}

BitmapID UserState::GetPersonalTowerBitmapId() {
    return this->personalTowerBitmapId;
}

BitmapID UserState::GetPersonalGoalBitmapId() {
    return this->personalGoalBitmapId;
}

BitmapID UserState::GetPersonalGoalSelectedId() {
    return this->personalGoalSelectedId;
}

BitmapID UserState::GetButtonId(UserStateButtonID button) {
    return this->buttonBitmapIds[static_cast<int>(button)];
}

bool UserState::GetSpawn(int spawnNum) {
    return this->spawnsActive[spawnNum - 1];
}

int UserState::GetSpawnValues() {
    int endValue = 0;
    if(this->spawnsActive[0]) {
        endValue |= 1;
    }

    if(this->spawnsActive[1]) {
        endValue |= 2;
    }

    if(this->spawnsActive[2]) {
        endValue |= 4;
    }

    return endValue;
}

bool UserState::HaveSpawnsChanged() {
    return this->spawnsChanged;
}

void UserState::SetSpawnsChanged(bool newValue) {
    this->spawnsChanged = newValue;
}

void UserState::ClickSpawn(int worldX, int worldY) {
    int spawnNum = this->CoordsToSpawn(worldX, worldY);
    this->ToggleSpawn(spawnNum);
}

void UserState::ToggleSpawn(int spawnNum) {
    this->spawnsActive[spawnNum - 1] = !(this->spawnsActive[spawnNum - 1]);
}

int UserState::CoordsToSpawn(int worldX, int worldY) {
    int relativeSpawn = 0;

    if(worldX == 0) {
        relativeSpawn = 3;
    } else if(worldY == 0) {
        relativeSpawn = 0;
    } else if(worldX == 24) {
        relativeSpawn = 1;
    } else {
        relativeSpawn = 2;
    }

    for(int i = 0; i < MECHANICS_NUM_GOALS; i++) {
        if((worldX == MECHANICS_WORLD_STATE_GOALS_XYS[relativeSpawn][i].first) &&
           (worldY == MECHANICS_WORLD_STATE_GOALS_XYS[relativeSpawn][i].second)) {
            return i + 1;
        }
    }

    std::cout << "Bad goal click" << std::endl;
    return 1;

}