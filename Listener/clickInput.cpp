#include "clickInput.h"

ClickInput::ClickInput() {
    this->clicked = false;
    this->x = 0;
    this->y = 0;
}

ClickInput::~ClickInput() {

}

bool ClickInput::GetClicked() {
    return this->clicked;
}

void ClickInput::SetClicked() {
    this->clicked = true;
}

void ClickInput::UnsetClicked() {
    this->clicked = false;
}

int ClickInput::GetX() {
    return this->x;
}

void ClickInput::SetX(int x) {
    this->x = x;
}

int ClickInput::GetY() {
    return this->y;
}

void ClickInput::SetY(int y) {
    this->y = y;
}

