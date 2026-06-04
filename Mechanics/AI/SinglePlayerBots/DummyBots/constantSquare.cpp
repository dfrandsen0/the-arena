#include "constantSquare.h"
#include "..\botTemplate.h"
#include "..\..\..\..\Listener\mainInput.h"
#include "..\..\..\..\Networking\Connections\infoPacket.h"
#include "..\..\..\gameState.h"

SPConstantSquare::SPConstantSquare(GameState* gameState, int playerNum) : SinglePlayerBot(gameState, playerNum) {

}

//does NOT delete any main inputs, but DOES delete the array
SPConstantSquare::~SPConstantSquare() {

}

InfoPacket* SPConstantSquare::GetInput(int currFrame) {
    if(this->frame >= currFrame) {
        return nullptr;
    }
    
    this->frame++;

    MainInput* currInput;
    if(this->delay == 0) {
        this->delay = this->betweenSquaresDelay;
        currInput = new MainInput(MainInputID::CREATE_SQUARE, 2, -1, -1);
    } else {
        (this->delay)--;
        currInput = new MainInput(MainInputID::DO_NOTHING, -1, -1, -1);
    }
    return new InfoPacket(currInput, this->frame - 1, this->playerNum);
}
