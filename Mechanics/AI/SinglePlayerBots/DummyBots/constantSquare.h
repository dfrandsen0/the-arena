#ifndef MECHANICS_AI_SINGLE_PLAYER_BOTS_CONSTANT_SQUARE_H_
#define MECHANICS_AI_SINGLE_PLAYER_BOTS_CONSTANT_SQUARE_H_

#include "..\botTemplate.h"
#include "..\..\..\..\Networking\Connections\infoPacket.h"
#include "..\..\..\..\Listener\mainInput.h"
#include "..\..\..\gameState.h"
#include "..\..\..\..\config.h"

class SPConstantSquare : public SinglePlayerBot {
public:
private:
    int frame = 0;
    int delay = 3; //default
    int betweenSquaresDelay = 60; //default
public:
    SPConstantSquare(GameState* gameState, int playerNum);
    ~SPConstantSquare();

    InfoPacket* GetInput(int currFrame);
};

#endif