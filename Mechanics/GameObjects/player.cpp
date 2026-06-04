#include "player.h"
#include "..\..\config.h"

Player::Player(int playerNum, int playerSide) {
    this->playerNum = playerNum;
    this->playerSide = playerSide;

    switch(playerNum) {
        case 1:
            this->color = PlayerColor::BLUE;
            break;
        case 2:
            this->color = PlayerColor::RED;
            break;
        case 3:
            this->color = PlayerColor::GREEN;
            break;
        default:
            this->color = PlayerColor::YELLOW;
            break;
    }
}

Player::~Player() {

}

int Player::GetNumTowers() {
    return this->numTowers;
}

void Player::DecrementTowers() {
    (this->numTowers)--;
}

void Player::IncrementTowers() {
    (this->numTowers)++;
}

int Player::GetMoney() {
    return this->money;
}

void Player::SpendMoney(int amount) {
    this->money -= amount;
    if(this->money < 0) {
        this->money = 0;
    }
}

void Player::IncrementMoney() {
    this->money += MECHANICS_MONEY_PER_FRAME;
    if(this->money > GRAPHICS_UI_NUM_MONEY * MECHANICS_MONEY_AMPLIFIER) {
        this->money = GRAPHICS_UI_NUM_MONEY * MECHANICS_MONEY_AMPLIFIER;
    }
}

int Player::GetLastMoney() {
    return this->lastMoney;
}

void Player::UpdateLastMoney() {
    this->lastMoney = this->money;
}

int Player::GetPlayerNum() {
    return this->playerNum;
}

int Player::GetPlayerSide() {
    return this->playerSide;
}

PlayerColor Player::GetColor() {
    return this->color;
}