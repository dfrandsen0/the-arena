#include <iostream>
#include <utility>

#include "mechanics.h"
#include "gameState.h"
#include "GameStructure\worldState.h"
#include "..\Resources\tools.h"
#include "..\Networking\frame.h"
#include "..\Networking\network.h"
#include "..\Listener\mainInput.h"

MechanicsModule::MechanicsModule() {

}

MechanicsModule::~MechanicsModule() {
    delete this->gameState;
    delete this->userState;
}

bool MechanicsModule::IsMultiplayer() {
    return this->isMultiplayer;
}

void MechanicsModule::SetMultiplayerStatus(bool isMultiplayer) {
    this->isMultiplayer = isMultiplayer;
}

int MechanicsModule::GetNumPlayers() {
    return this->numPlayers;
}

void MechanicsModule::SetNumPlayers(int numPlayers) {
    this->numPlayers = numPlayers;
}

int MechanicsModule::GetThisPlayer() {
    return this->thisPlayer;
}

void MechanicsModule::SetThisPlayer(int thisPlayer) {
    this->thisPlayer = thisPlayer;
}

void MechanicsModule::InitStates(int numPlayers, std::pair<int, int>* playersToSides) {
    if(this->gameState != nullptr) {
        delete this->gameState;
    }

    this->gameState = new GameState(numPlayers, playersToSides);

    if(this->userState != nullptr) {
        delete this->userState;
    }

    this->userState = new UserState(this->thisPlayer);
}

GameState* MechanicsModule::GetGameState() {
    return this->gameState;
}

UserState* MechanicsModule::GetUserState() {
    return this->userState;
}

void MechanicsModule::UpdateNetworkingModule(NetworkingModule* networkingModule) {
    this->networkingModule = networkingModule;
}

void MechanicsModule::UpdateGameState(Frame* frame) {
    if(this->gameOver) {
        delete frame;
        return;
    }

    this->gameState->IncrementPlayerMoney();

    MainInput** inputs = frame->GetInputs();
    for(int i = 0; i < MECHANICS_MAX_NUM_PLAYERS; i++) {
        if((inputs[i] == nullptr) ||
           (inputs[i]->type == MainInputID::DO_NOTHING) ||
            this->gameState->IsPlayerOut(i + 1)) {
            continue;
        }

        this->gameState->HandleInput(inputs[i], i + 1);
    }
    
    //update towers
    this->gameState->UpdateTowers();

    //update walls
    this->gameState->UpdateWalls();
    
    //update AI
    this->gameState->GetAIModule()->GetPathFinder()->UpdatePathFinding();

    //collisions here. that way they show "touching" for a full frame
    //  and your buckets don't have to delete them
    this->gameState->CalculateCollisions();

    //update units
    this->gameState->UpdateUnits();

    //update goals
    if(this->gameState->UpdateGoals(this->thisPlayer)) {
        this->networkingModule->UpdateLivePlayers(this->gameState);

        if(this->gameState->IsPlayerOut(this->thisPlayer)) {
            this->userState->KillThisPlayer();
        }

        if(this->IsGameOver()) {
            this->gameOver = true;
            this->userState->KillThisPlayer();
        }
    }

    delete frame;
}

std::pair<int, int>* MechanicsModule::FindSPStartingValues(int* thisPlayer) {
    int players[MECHANICS_MAX_NUM_PLAYERS] = {1, 2, 3, 4};
    int sides[MECHANICS_MAX_NUM_PLAYERS] = {0, 1, 2, 3};

    Tools::ShuffleArray(sides, MECHANICS_MAX_NUM_PLAYERS);

    std::pair<int, int>* playerSidePairs = new std::pair<int, int>[MECHANICS_MAX_NUM_PLAYERS];

    for(int i = 0; i < MECHANICS_MAX_NUM_PLAYERS; i++) {
        playerSidePairs[i].first = players[i];
        playerSidePairs[i].second = sides[i];
    }

    Tools::ShuffleArray(playerSidePairs, MECHANICS_MAX_NUM_PLAYERS);

    (*thisPlayer) = playerSidePairs[0].first;

    return playerSidePairs;
}

bool MechanicsModule::IsGameOver() {
    int playersAlive = 0;
    for(int i = 1; i <= MECHANICS_MAX_NUM_PLAYERS; i++) {
        if(!this->gameState->IsPlayerOut(i)) {
            playersAlive++;
        }
    }

    if(playersAlive <= 1) {
        return true;
    }
    return false;
}

void MechanicsModule::SetGameOver(bool newValue) {
    this->gameOver = newValue;
}

bool MechanicsModule::GetGameOverValue() {
    return this->gameOver;
}
