#ifndef LISTENER_CLICK_INPUT_H_
#define LISTENER_CLICK_INPUT_H_

#include <utility>

class ClickInput {
private:
    bool clicked;
    int x;
    int y;

public:

    ClickInput();
    ~ClickInput();

    bool GetClicked();
    void SetClicked();
    void UnsetClicked();

    int GetX();
    void SetX(int x);

    int GetY();
    void SetY(int y);
};

#endif