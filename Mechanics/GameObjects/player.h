#ifndef MECHANICS_GAME_OBJECTS_PLAYER_H_
#define MECHANICS_GAME_OBJECTS_PLAYER_H_

#include "..\..\config.h"

class Player {
public:
private:
    int numTowers = 0;
    int lastMoney = MECHANICS_PLAYER_STARTING_MONEY;
    int money = MECHANICS_PLAYER_STARTING_MONEY;
    int playerNum;
    int playerSide;

    PlayerColor color;
public:
    Player(int playerNum, int playerSide);
    ~Player();

    int GetNumTowers();
    void DecrementTowers();
    void IncrementTowers();

    int GetMoney();
    void SpendMoney(int amount);
    void IncrementMoney();

    int GetLastMoney();
    void UpdateLastMoney();

    int GetPlayerNum();
    int GetPlayerSide();
    PlayerColor GetColor();
private:
};

#endif