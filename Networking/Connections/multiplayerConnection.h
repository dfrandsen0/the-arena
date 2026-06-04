#ifndef NETWORKING_CONNECTIONS_MULTIPLAYER_CONNECTION_H_
#define NETWORKING_CONNECTIONS_MULTIPLAYER_CONNECTION_H_

#include <winsock2.h>
#include <ws2tcpip.h>

#include "connection.h"
#include "infoPacket.h"
#include "..\..\config.h"

class MultiplayerConnection : public Connection {
private:
    SOCKET serverSocket;
    char* buffer = nullptr;
    int bufferSize = NETWORKING_MULTIPLAYER_BUFFER_SIZE;
    int currLength = 0;
public:
    MultiplayerConnection(SOCKET serverSocket);
    ~MultiplayerConnection();

    InfoPacket** ReceiveInput(int currFrame, int* numPackets);
    void SendInput(InfoPacket* packet);
    void BreakConnection();

private:
    bool SendOut(const std::string& msg);
};

#endif