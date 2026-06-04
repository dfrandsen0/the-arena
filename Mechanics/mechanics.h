#ifndef MECHANICS_H_
#define MECHANICS_H_

#include <utility>

#include "gameState.h"
#include "userState.h"
#include "..\Networking\frame.h"
#include "..\Networking\network.h"
#include "GameStructure\worldState.h"

class MechanicsModule {
public:
private:
    GameState* gameState = nullptr;
    UserState* userState = nullptr;
    NetworkingModule* networkingModule = nullptr;
    bool isMultiplayer = false;
    int numPlayers = -1;

    int thisPlayer;

    bool gameOver = false;
public:
    MechanicsModule();
    ~MechanicsModule();

    bool IsMultiplayer();
    void SetMultiplayerStatus(bool isMultiplayer);

    int GetNumPlayers();
    void SetNumPlayers(int numPlayers);

    int GetThisPlayer();
    void SetThisPlayer(int thisPlayer);

    void InitStates(int numPlayers, std::pair<int, int>* playersToSides);
    GameState* GetGameState();
    UserState* GetUserState();
    void UpdateNetworkingModule(NetworkingModule* networkingModule);

    void UpdateGameState(Frame* frame);

    std::pair<int, int>* FindSPStartingValues(int* thisPlayer);

    bool IsGameOver();
    void SetGameOver(bool newValue);
    bool GetGameOverValue();
private:
};

#endif