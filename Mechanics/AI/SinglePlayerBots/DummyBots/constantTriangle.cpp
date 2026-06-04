#include "constantTriangle.h"
#include "..\botTemplate.h"
#include "..\..\..\..\Listener\mainInput.h"
#include "..\..\..\..\Networking\Connections\infoPacket.h"
#include "..\..\..\gameState.h"

SPConstantTriangle::SPConstantTriangle(GameState* gameState, int playerNum) : SinglePlayerBot(gameState, playerNum) {

}

//does NOT delete any main inputs, but DOES delete the array
SPConstantTriangle::~SPConstantTriangle() {

}

InfoPacket* SPConstantTriangle::GetInput(int currFrame) {
    if(this->frame >= currFrame) {
        return nullptr;
    }

    this->frame++;

    MainInput* currInput;
    if(this->delay == 0) {
        this->delay = this->betweenTrianglesDelay;
        currInput = new MainInput(MainInputID::CREATE_TRIANGLE, 2, -1, -1);
    } else {
        (this->delay)--;
        currInput = new MainInput(MainInputID::DO_NOTHING, -1, -1, -1);
    }

    return new InfoPacket(currInput, this->frame - 1, this->playerNum);
}
