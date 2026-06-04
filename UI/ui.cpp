#include <utility>

#include "ui.h"

#include "..\Graphics\graphics.h"
#include "..\Mechanics\mechanics.h"
#include "..\Listener\clickInput.h"
#include "..\config.h"

UIModule::UIModule(GraphicsModule* graphicsModule, MechanicsModule* mechanicsModule) {
    this->graphicsModule = graphicsModule;
    this->mechanicsModule = mechanicsModule;
}

UIModule::~UIModule() {

}

void UIModule::HandleInput(ClickInput* clickInput) {
    if(!(clickInput->GetClicked()) || this->delay >= 0) {
        return;
    }
    
    clickInput->UnsetClicked();

    this->pressed = this->TranslateInput(clickInput->GetX(), clickInput->GetY());
    if(this->pressed == -1) {
        return;
    }

    this->graphicsModule->DrawUI(this->state, this->pressed);
    this->delay = UI_DEFAULT_DELAY;

    switch(this->pressed) {
        case 0:
            this->nextRenderState = UIState::SINGLE_PLAYER;
            this->mechanicsModule->SetMultiplayerStatus(false);
            break;
        case 1:
            this->nextRenderState = UIState::MULTI_PLAYER;
            this->mechanicsModule->SetMultiplayerStatus(true);
            break;
        case 2:
            this->nextRenderState = UIState::QUIT;
            break;
        case 3:
            this->mechanicsModule->SetNumPlayers(2);
            this->nextRenderState = UIState::LOADING;
            break;
        case 4:
            this->mechanicsModule->SetNumPlayers(3);
            this->nextRenderState = UIState::LOADING;
            break;
        case 5:
            this->mechanicsModule->SetNumPlayers(4);
            this->nextRenderState = UIState::LOADING;
            break;
        case 6:
            this->nextRenderState = UIState::MAIN_MENU;
            break;
        default:
            break;
    }
}

void UIModule::ActOnRender() {
    this->graphicsModule->DrawUI(this->nextRenderState, -1);
}

UIState UIModule::GetState() {
    return this->state;
}

void UIModule::AdvanceState() {
    this->state = this->nextRenderState;
}

void UIModule::Reset() {
    this->state = UIState::MAIN_MENU;
    this->nextRenderState = UIState::NONE;
    this->delay = -1;
    this->pressed = -1;
}

int UIModule::GetDelay() {
    return this->delay;
}

void UIModule::SetDelay(int delay) {
    this->delay = delay;
}

void UIModule::DecrementDelay() {
    if(this->delay > -1) {
        (this->delay)--;
    }
}

int UIModule::GetPressed() {
    return this->pressed;
}

void UIModule::SetPressed(int pressed) {
    this->pressed = pressed;
}

int UIModule::TranslateInput(int x, int y) {
    switch(this->state) {
        case UIState::MAIN_MENU:
            if(this->graphicsModule->WithinButton(x, y, UI_BUTTONS_XYS[0][0].first, UI_BUTTONS_XYS[0][0].second, BitmapID::UI_BUTTON_TEMPLATE)) {
                return 0;
            }

            if(this->graphicsModule->WithinButton(x, y, UI_BUTTONS_XYS[0][1].first, UI_BUTTONS_XYS[0][1].second, BitmapID::UI_BUTTON_TEMPLATE)) {
                return 1;
            }

            if(this->graphicsModule->WithinButton(x, y, UI_BUTTONS_XYS[0][2].first, UI_BUTTONS_XYS[0][2].second, BitmapID::UI_X_BUTTON)) {
                return 2;
            }

            break;
        case UIState::SINGLE_PLAYER:
        case UIState::MULTI_PLAYER:
            for(int i = 0; i < 3; i++) {
                if(this->graphicsModule->WithinButton(x, y, UI_BUTTONS_XYS[1][i].first, UI_BUTTONS_XYS[1][i].second, BitmapID::UI_BUTTON_TEMPLATE)) {
                    return i + 3;
                }
            }

            if(this->graphicsModule->WithinButton(x, y, UI_BUTTONS_XYS[1][3].first, UI_BUTTONS_XYS[1][3].second, BitmapID::UI_BACK_BUTTON)) {
                return 6;
            }
            break;
        default:
            break;
    }

    return -1;
}

