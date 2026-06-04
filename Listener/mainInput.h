#ifndef LISTENER_MAIN_INPUT_H_
#define LISTENER_MAIN_INPUT_H_

#include "..\config.h"

struct MainInput {
public:
    MainInputID type;
    int xParam;
    int yParam;
    int idParam;

    MainInput(MainInputID type, int xParam, int yParam, int idParam);
    ~MainInput();
};

#endif