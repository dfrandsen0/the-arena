#ifndef MECHANICS_AI_SINGLE_PLAYER_BOTS_CONSTANT_CIRCLE_H_
#define MECHANICS_AI_SINGLE_PLAYER_BOTS_CONSTANT_CIRCLE_H_

#include "..\botTemplate.h"
#include "..\..\..\..\Networking\Connections\infoPacket.h"
#include "..\..\..\..\Listener\mainInput.h"
#include "..\..\..\gameState.h"
#include "..\..\..\..\config.h"

class SPConstantCircle : public SinglePlayerBot {
public:
private:
    int frame = 0;
    int delay = 3; //default
    int betweenCirclesDelay = 60; //default
public:
    SPConstantCircle(GameState* gameState, int playerNum);
    ~SPConstantCircle();

    InfoPacket* GetInput(int currFrame);
};

#endif