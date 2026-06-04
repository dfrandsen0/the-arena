#ifndef MECHANICS_AI_SINGLE_PLAYER_BOTS_BOT_TEMPLATE_H_
#define MECHANICS_AI_SINGLE_PLAYER_BOTS_BOT_TEMPLATE_H_

#include "..\..\gameState.h"
#include "..\..\..\Networking\Connections\infoPacket.h"
#include "..\..\..\config.h"

class SinglePlayerBot {
public:
protected:
    //DO NOT delete gameState!
    GameState* gameState = nullptr;
    int playerNum;
    int lastSent = -1;
public:
    explicit SinglePlayerBot(GameState* gameState, int playerNum);
    virtual ~SinglePlayerBot();

    virtual InfoPacket* GetInput(int currFrame) = 0;
};

#endif