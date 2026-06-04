#ifndef NETWORKING_INFO_PACKET_H_
#define NETWORKING_INFO_PACKET_H_

#include "..\..\Listener\mainInput.h"

struct InfoPacket {
public:
    MainInput* input;
    int frameNum;
    int playerNum;

    InfoPacket(MainInput* input, int frameNum, int playerNum);
    ~InfoPacket();
};

#endif