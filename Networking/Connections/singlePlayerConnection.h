#ifndef NETWORKING_CONNECTIONS_SINGLE_PLAYER_CONNECTION_H_
#define NETWORKING_CONNECTIONS_SINGLE_PLAYER_CONNECTION_H_

#include "connection.h"
#include "infoPacket.h"
#include "..\..\Mechanics\AI\SinglePlayerBots\botTemplate.h"

class SinglePlayerConnection : public Connection {
public:
protected:
private:
    SinglePlayerBot** bots;
public:
    SinglePlayerConnection();
    ~SinglePlayerConnection();

    InfoPacket** ReceiveInput(int currFrame, int* numPackets);
    void SendInput(InfoPacket* packet);
    void BreakConnection();

    void AddBot(SinglePlayerBot* bot, int playerNum);
};

#endif