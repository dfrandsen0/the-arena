#ifndef UI_H_
#define UI_H_

#include "..\Graphics\graphics.h"
#include "..\Mechanics\mechanics.h"
#include "..\Listener\clickInput.h"
#include "..\config.h"

class UIModule {
public:
private:
    GraphicsModule* graphicsModule = nullptr;
    MechanicsModule* mechanicsModule = nullptr;

    UIState state = UIState::MAIN_MENU;
    int delay = -1;
    int pressed = -1;

    UIState nextRenderState = UIState::NONE;

public:
    UIModule(GraphicsModule* graphicsModule, MechanicsModule* mechanicsModule);
    ~UIModule();

    void HandleInput(ClickInput* clickInput);

    void ActOnRender();

    UIState GetState();
    void AdvanceState();
    void Reset();

    int GetDelay();
    void SetDelay(int delay);
    void DecrementDelay();

    int GetPressed();
    void SetPressed(int pressed);
    
private:
    int TranslateInput(int x, int y);
};

#endif