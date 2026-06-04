#ifndef MECHANICS_AI_SINGLE_PLAYER_BOTS_DO_NOTHING_H_
#define MECHANICS_AI_SINGLE_PLAYER_BOTS_DO_NOTHING_H_

#include "..\botTemplate.h"
#include "..\..\..\..\Networking\Connections\infoPacket.h"
#include "..\..\..\gameState.h"

class SPDoNothing : public SinglePlayerBot {
public:
private:
    int frame = 0;
public:
    SPDoNothing(GameState* gameState, int playerNum);
    ~SPDoNothing();

    InfoPacket* GetInput(int currFrame);
private:

};

#endif