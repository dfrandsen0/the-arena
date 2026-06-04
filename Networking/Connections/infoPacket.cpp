#include "infoPacket.h"
#include "..\..\Listener\mainInput.h"

InfoPacket::InfoPacket(MainInput* input, int frameNum, int playerNum) {
    this->input = input;
    this->frameNum = frameNum;
    this->playerNum = playerNum;
}

//does NOT delete the maininput
InfoPacket::~InfoPacket() {
    
}
