#include "doNothing.h"
#include "..\botTemplate.h"
#include "..\..\..\..\Networking\Connections\infoPacket.h"
#include "..\..\..\gameState.h"

SPDoNothing::SPDoNothing(GameState* gameState, int playerNum) : SinglePlayerBot(gameState, playerNum) {

}

SPDoNothing::~SPDoNothing() {

}

InfoPacket* SPDoNothing::GetInput(int currFrame) {
    if(this->frame >= currFrame) {
        return nullptr;
    }

    this->frame++;
    return new InfoPacket(new MainInput(MainInputID::DO_NOTHING, -1, -1, -1), this->frame - 1, this->playerNum);
}
