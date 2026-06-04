#ifndef NETWORKING_CONNECTIONS_CONNECTION_H_
#define NETWORKING_CONNECTIONS_CONNECTION_H_

#include "infoPacket.h"

class Connection {
public:
    explicit Connection();
    virtual ~Connection();

    virtual InfoPacket** ReceiveInput(int currFrame, int* numPackets) = 0;
    virtual void SendInput(InfoPacket* packet) = 0;
    virtual void BreakConnection() = 0;
};

#endif