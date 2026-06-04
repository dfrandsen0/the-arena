#include <iostream>

#include "frame.h"
#include "..\Listener\mainInput.h"
#include "..\config.h"

Frame::Frame(int id) {
    this->inputs = new MainInput*[MECHANICS_MAX_NUM_PLAYERS];
    for(int i = 0; i < MECHANICS_MAX_NUM_PLAYERS; i++) {
        this->inputs[i] = nullptr;
    }

    this->id = id;
}

Frame::~Frame() {
    for(int i = 0; i < MECHANICS_MAX_NUM_PLAYERS; i++) {
        delete this->inputs[i];
    }
    delete[] this->inputs;
}

void Frame::AddToFrame(MainInput* newInput, int playerNum) {
    //translate from {1 - 4} (possible player numbers) to {0 - 3} (the array)
    playerNum -= 1;

    if(this->inputs[playerNum] != nullptr) {
        delete this->inputs[playerNum];
    }
    this->inputs[playerNum] = newInput;
}

MainInput** Frame::GetInputs() {
    return this->inputs;
}

void Frame::PrintFrame() {
    std::cout << this->id << "  |  ";
    for(int i = 0; i < MECHANICS_MAX_NUM_PLAYERS; i++) {
        if(this->inputs[i] == nullptr) {
            std::cout << "n  ";
        } else {
            std::cout << static_cast<int>(this->inputs[i]->type) << "  ";
        }
    }

    std::cout << std::endl;
}