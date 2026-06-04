#ifndef NETWORKING_NETWORK_H_
#define NETWORKING_NETWORK_H_

#include <winsock2.h>
#include <ws2tcpip.h>
#include <utility>

#include "frame.h"
#include "..\Listener\mainInput.h"
#include "Connections\connection.h"
#include "Connections\infoPacket.h"
#include "..\Mechanics\gameState.h"
#include "..\config.h"

class NetworkingModule {
public:
private:
    int thisPlayer = 0;
    bool multiplayer = false;

    Connection* connection = nullptr;

    Frame** frames = nullptr;
    int baseFrameNum = 0;

    InfoPacket** aheadPackets = nullptr;
    int aheadPacketsLength = 0;

    bool playersAlive[MECHANICS_MAX_NUM_PLAYERS] = {false, false, false, false};
    SOCKET serverSock;

public:
    NetworkingModule();
    ~NetworkingModule();

    void InitNetworkingFrames(int numPlayers, std::pair<int, int>* playersToSides, int thisPlayer);

    void SetMultiplayer(bool isMultiplayer);
    std::pair<int, int>* ConnectToServer(int numPlayers, int* thisPlayer);

    void CreateMultiplayerConnection(int numPlayers, std::pair<int, int>* playersToSides);
    void ConnectBots(GameState* gameState, int numPlayers, std::pair<int, int>* playersToSides);

    void ReadConnection();
    bool IsFrameFilled(int frameNum);

    void UpdateLivePlayers(GameState* gameState);

    void AddToFrames(InfoPacket* packet);

    Frame* ProcessFrame(MainInput* mainInput, int frameNum);
   
    void BreakConnections();

private:
    bool SendToServer(SOCKET sock, const std::string& mst);

    void PrintFrames();
};

#endif