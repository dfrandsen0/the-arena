#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <utility>

#include "..\Listener\mainInput.h"
#include "network.h"
#include "frame.h"
#include "Connections\connection.h"
#include "Connections\singlePlayerConnection.h"
#include "Connections\multiplayerConnection.h"
#include "Connections\infoPacket.h"
#include "..\Mechanics\AI\SinglePlayerBots\botTemplate.h"
#include "..\Mechanics\AI\SinglePlayerBots\DummyBots\constantSquare.h"
#include "..\Mechanics\AI\SinglePlayerBots\DummyBots\constantTriangle.h"
#include "..\Mechanics\AI\SinglePlayerBots\DummyBots\constantCircle.h"
#include "..\Mechanics\AI\SinglePlayerBots\DummyBots\doNothing.h"
#include "..\Mechanics\gameState.h"
#include "..\config.h"

NetworkingModule::NetworkingModule() {
    this->frames = new Frame*[NETWORKING_FRAME_DELAY];
    this->aheadPackets = new InfoPacket*[NETWORKING_MAX_PACKETS];

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cout << "WSAStartup failed" << std::endl;
    }
}

NetworkingModule::~NetworkingModule() {
    for(int i = 0; i < NETWORKING_FRAME_DELAY; i++) {
        delete this->frames[i];
    }
    delete[] this->frames;

    for(int i = 0; i < this->aheadPacketsLength; i++) {
        delete this->aheadPackets[i];
    }
    delete[] this->aheadPackets;

    delete this->connection;
}

void NetworkingModule::InitNetworkingFrames(int numPlayers, std::pair<int, int>* playersToSides, int thisPlayer) {
    this->thisPlayer = thisPlayer;

    int startFrame = -1 * (NETWORKING_FRAME_DELAY - 1);

    for(int i = 0; i < NETWORKING_FRAME_DELAY; i++) {
        frames[i] = new Frame(startFrame);

        for(int j = 0; j < numPlayers; j++) {
            MainInput* newInput = new MainInput(MainInputID::DO_NOTHING, -1, -1, -1);
            frames[i]->AddToFrame(newInput, playersToSides[j].first);
        }

        startFrame++;
    }

    for(int i = 0; i < numPlayers; i++) {
        this->playersAlive[playersToSides[i].first - 1] = true;
    }

    this->baseFrameNum = startFrame;
}

void NetworkingModule::SetMultiplayer(bool isMultiplayer) {
    this->multiplayer = isMultiplayer;
}

std::pair<int, int>* NetworkingModule::ConnectToServer(int numPlayers, int* thisPlayer) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cout << "Socket creation failed" << std::endl;
        WSACleanup();
        return nullptr;
    }

    std::string serverIp = "100.48.19.251";    // change me
    unsigned short serverPort = 8080;          // match the server

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = inet_addr(serverIp.c_str());

    u_long mode = 1;
    ioctlsocket(sock, FIONBIO, &mode);

    std::cout << "Connecting to server..." << std::endl;
    int c = connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));

    if(c == 0) {
        std::cout << "Connected." << std::endl;
    } else if(WSAGetLastError() == WSAEWOULDBLOCK) {
        fd_set writes;
        FD_ZERO(&writes);
        FD_SET(sock, &writes);

        timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        int nready = select(0, nullptr, &writes, nullptr, &timeout);

        if(nready > 0) {
            int optVal;
            int optLen = sizeof(optVal);
            getsockopt(sock, SOL_SOCKET, SO_ERROR, (char*)&optVal, &optLen);

            if(optVal == 0) {
                std::cout << "Connected." << std::endl;
            } else {
                std::cout << "Failed." << std::endl;
                closesocket(sock);
                WSACleanup();
                return nullptr;
            }
        } else {
            std::cout << "Select failed or timed out" << std::endl;
            closesocket(sock);
            WSACleanup();
            return nullptr;
        }
    } else {
        std::cout << "Connection failed" << std::endl;
        closesocket(sock);
        WSACleanup();
        return nullptr;
    }

    std::string joinMsg = "JOIN " + std::to_string(numPlayers) + "\n";
    if (!SendToServer(sock, joinMsg)) {
        std::cout << "Send failed" << std::endl;
        closesocket(sock);
        WSACleanup();
        return nullptr;
    }

    std::cout << "Waiting for match..." << std::endl;

    char buffer[2048];
    int recvSize;
    for(;;) {
        recvSize = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if(recvSize <= 0 && WSAGetLastError() != WSAEWOULDBLOCK) {
            std::cout << "Server disconnected." << std::endl;
            closesocket(sock);
            WSACleanup();
            return nullptr;
        }

        if(recvSize > 0) {
            break;
        }
        Sleep(200);
    }
    
    buffer[recvSize] = '\0';
    std::string response(buffer);

    std::cout << std::endl << "server Response:" << std::endl << response << std::endl << std::endl;
    if(response.length() < 23 || (response.find("MATCH") == std::string::npos)) {
        std::cout << "Server busy!" << std::endl;
        closesocket(sock);
        WSACleanup();
        return nullptr;
    }

    std::pair<int, int>* playersToSides = new std::pair<int, int>[MECHANICS_MAX_NUM_PLAYERS] {
        {response[6] - 48, response[14] - 48},
        {response[8] - 48, response[16] - 48}, 
        {response[10] - 48, response[18] - 48},
        {response[12] - 48, response[20] - 48}
    };

    this->serverSock = sock;
    *thisPlayer = response[22] - 48;

    return playersToSides;
}

void NetworkingModule::CreateMultiplayerConnection(int numPlayers, std::pair<int, int>* playersToSides) {
    std::cout << "createMultiplayerConnection" << std::endl;
    if(this->connection != nullptr) {
        delete this->connection;
    }

    this->connection = new MultiplayerConnection(this->serverSock);
}

void NetworkingModule::ConnectBots(GameState* gameState, int numPlayers, std::pair<int, int>* playersToSides) {
    if(this->connection != nullptr) {
        delete this->connection;
    }

    SinglePlayerConnection* spc = new SinglePlayerConnection();
    
    //tmp
    if(numPlayers == 4) {
        spc->AddBot(new SPConstantSquare(gameState, playersToSides[1].first), playersToSides[1].first);
        spc->AddBot(new SPConstantTriangle(gameState, playersToSides[2].first), playersToSides[2].first);
        spc->AddBot(new SPConstantCircle(gameState, playersToSides[3].first), playersToSides[3].first);
        this->connection = spc;
        return;
    }
    //

    for(int i = 1; i < numPlayers; i++) {
        spc->AddBot(new SPConstantSquare(gameState, playersToSides[i].first), playersToSides[i].first);
    }

    this->connection = spc;
}

void NetworkingModule::ReadConnection() {
    int numPackets = 0;

    for(int i = 0; i < this->aheadPacketsLength; i++) {
        if(this->aheadPackets[i]->frameNum <= this->baseFrameNum) {
            this->AddToFrames(aheadPackets[i]);
            this->aheadPackets[i] = this->aheadPackets[aheadPacketsLength];
            this->aheadPackets[aheadPacketsLength] = nullptr;
            (this->aheadPacketsLength)--;
            i--;
        }
    }

    InfoPacket** packetList = this->connection->ReceiveInput(this->baseFrameNum, &numPackets);

    for(int i = 0; i < numPackets; i++) {
        this->AddToFrames(packetList[i]);
    }

    delete[] packetList;
}

bool NetworkingModule::IsFrameFilled(int frameNum) {
    for(int i = 0; i < MECHANICS_MAX_NUM_PLAYERS; i++) {
        if((i + 1 != this->thisPlayer) &&
           (this->playersAlive[i]) &&
           (this->frames[NETWORKING_FRAME_DELAY - 1]->GetInputs()[i] == nullptr)) {
            return false;
        }
    }

    return true;
}

void NetworkingModule::UpdateLivePlayers(GameState* gameState) {
    for(int i = 0; i < MECHANICS_MAX_NUM_PLAYERS; i++) {
        this->playersAlive[i] = !(gameState->IsPlayerOut(i + 1));
    }
}

void NetworkingModule::AddToFrames(InfoPacket* packet) {
    if(packet->frameNum > this->baseFrameNum) {
        this->aheadPackets[aheadPacketsLength] = packet;
        aheadPacketsLength++;
    } else {
        this->frames[this->baseFrameNum - packet->frameNum]->AddToFrame(packet->input, packet->playerNum);
    }

    delete packet;
}

Frame* NetworkingModule::ProcessFrame(MainInput* mainInput, int frameNum) {
    Frame* currentFrame = this->frames[NETWORKING_FRAME_DELAY - 1];

    for(int i = NETWORKING_FRAME_DELAY - 1; i > 0; i--) {
        this->frames[i] = this->frames[i - 1];
    }

    this->frames[0] = new Frame(frameNum);
    this->frames[0]->AddToFrame(mainInput, this->thisPlayer);
    
    InfoPacket* thisPacket = new InfoPacket(mainInput, this->baseFrameNum, this->thisPlayer);
    this->connection->SendInput(thisPacket);
    delete thisPacket;

    this->baseFrameNum++;

    return currentFrame;
}

void NetworkingModule::BreakConnections() {
    this->connection->BreakConnection();
}

bool NetworkingModule::SendToServer(SOCKET sock, const std::string& msg) {
    int totalSent = 0;
    int toSend = (int)msg.size();
    while (totalSent < toSend) {
        int sent = send(sock, msg.c_str() + totalSent, toSend - totalSent, 0);
        if (sent <= 0) return false;
        totalSent += sent;
    }
    return true;
}

void NetworkingModule::PrintFrames() {
    std::cout << std::endl << "Frame" << std::endl << std::endl;
    for(int i = 0; i < NETWORKING_FRAME_DELAY; i++) {
        this->frames[i]->PrintFrame();
    }
    std::cout << std::endl;
}
