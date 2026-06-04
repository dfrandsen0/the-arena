#ifndef NETWORKING_FRAME_H_
#define NETWORKING_FRAME_H_

#include "..\Listener\mainInput.h"

class Frame {
public:
private:
    int id;
    MainInput** inputs = nullptr;
    //hash?
public:
    Frame(int id);
    ~Frame();

    void AddToFrame(MainInput* newInput, int playerNum);
    MainInput** GetInputs();

    void PrintFrame();
private:
};

#endif