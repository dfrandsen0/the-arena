#include <iostream>

#include "singlePlayerConnection.h"
#include "InfoPacket.h"
#include "..\..\Mechanics\AI\SinglePlayerBots\botTemplate.h"

SinglePlayerConnection::SinglePlayerConnection() {
    this->bots = new SinglePlayerBot*[MECHANICS_MAX_NUM_PLAYERS];
    for(int i = 0; i < MECHANICS_MAX_NUM_PLAYERS; i++) {
        this->bots[i] = nullptr;
    }
}

SinglePlayerConnection::~SinglePlayerConnection() {
    for(int i = 0; i < MECHANICS_MAX_NUM_PLAYERS; i++) {
        delete this->bots[i];
    }
    delete this->bots;
}

InfoPacket** SinglePlayerConnection::ReceiveInput(int currFrame, int* numPackets) {
    InfoPacket** packetList = new InfoPacket*[NETWORKING_MAX_PACKETS];
    *numPackets = 0;

    for(int i = 0; i < MECHANICS_MAX_NUM_PLAYERS; i++) {
        if(this->bots[i] != nullptr) {
            packetList[*numPackets] = this->bots[i]->GetInput(currFrame);
            if(packetList[*numPackets] != nullptr) {
                (*numPackets)++;
            }
        }
    }

    return packetList;
}

void SinglePlayerConnection::SendInput(InfoPacket* packet) {
    //empty, SP can't use that information because it's not common knowledge.
    //  At least, without cheating.
    return;
}

void SinglePlayerConnection::BreakConnection() {

}

void SinglePlayerConnection::AddBot(SinglePlayerBot* bot, int playerNum) {
    this->bots[playerNum - 1] = bot;
}