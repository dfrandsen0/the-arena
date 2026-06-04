#include "botTemplate.h"
#include "..\..\gameState.h"

SinglePlayerBot::SinglePlayerBot(GameState* gameState, int playerNum) {
    this->gameState = gameState;
    this->playerNum = playerNum;
}

SinglePlayerBot::~SinglePlayerBot() {
    
}
