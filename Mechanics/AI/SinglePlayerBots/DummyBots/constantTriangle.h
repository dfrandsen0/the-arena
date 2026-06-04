#ifndef MECHANICS_AI_SINGLE_PLAYER_BOTS_CONSTANT_TRIANGLE_H_
#define MECHANICS_AI_SINGLE_PLAYER_BOTS_CONSTANT_TRIANGLE_H_

#include "..\botTemplate.h"
#include "..\..\..\..\Networking\Connections\infoPacket.h"
#include "..\..\..\..\Listener\mainInput.h"
#include "..\..\..\gameState.h"
#include "..\..\..\..\config.h"

class SPConstantTriangle : public SinglePlayerBot {
public:
private:
    int frame = 0;
    int delay = 3; //default
    int betweenTrianglesDelay = 60; //default
public:
    SPConstantTriangle(GameState* gameState, int playerNum);
    ~SPConstantTriangle();

    InfoPacket* GetInput(int currFrame);
};

#endif