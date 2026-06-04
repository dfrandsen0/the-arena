#include <iostream>

#include "mainInput.h"
#include "..\config.h"

MainInput::MainInput(MainInputID type, int xParam, int yParam, int idParam) {
    this->type = type;
    this->xParam = xParam;
    this->yParam = yParam;
    this->idParam = idParam;
}

MainInput::~MainInput() {

}
