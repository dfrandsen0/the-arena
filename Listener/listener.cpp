#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <windowsx.h>
#include <iostream>

#include "listener.h"
#include "mainInput.h"
#include "clickInput.h"
#include "..\Mechanics\userState.h"
#include "..\Mechanics\gameState.h"
#include "..\config.h"

MSG ListenerModule::msg = {};
bool ListenerModule::running = true;

ListenerModule::ListenerModule() {
    //constructor; empty
    // set private so that no instantiations can exist
}

LRESULT CALLBACK ListenerModule::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    ClickInput* clickInput = (ClickInput*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    
    switch (msg) {
        case WM_LBUTTONDOWN:
            if(clickInput == nullptr) {
                break;
            }

            clickInput->SetClicked();
            clickInput->SetX(GET_X_LPARAM(lParam));
            clickInput->SetY(GET_Y_LPARAM(lParam));
            break;
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
            break;
        }        
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void ListenerModule::CheckInput() {
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            ListenerModule::running = false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

bool ListenerModule::IsRunning() {
    return ListenerModule::running;
}

MainInput* ListenerModule::TranslateMainInput(ClickInput* clickInput, MechanicsModule* mechanicsModule) {
    UserState* userState = mechanicsModule->GetUserState();
    GameState* gameState = mechanicsModule->GetGameState();

    if(userState->GetDelay() > 0) {
        clickInput->UnsetClicked();
        userState->DecrementDelay();
        return new MainInput(MainInputID::DO_NOTHING, -1, -1, -1);
    }

    if(userState->GetSelected() == UserSelect::PLACE_WALL) {
        userState->SetSelected(UserSelect::WALL_MODE);
    }

    if(!(userState->GetSelected() == UserSelect::WALL_MODE ||
         userState->GetSelected() == UserSelect::TOWER_MODE ||
         userState->GetSelected() == UserSelect::TOWER ||
         userState->GetSelected() == UserSelect::NONE)) {

        userState->SetSelected(UserSelect::NONE);
    }

    if(!(clickInput->GetClicked())) {
        return new MainInput(MainInputID::DO_NOTHING, -1, -1, -1);
    }

    MainClickID mainClickType = ListenerModule::DetermineClicked(clickInput, gameState, userState->ThisPlayerIsAlive());

    clickInput->UnsetClicked();
    
    switch(mainClickType) {
        case MainClickID::NONE:
            break;
        case MainClickID::WALL_BUTTON:
            return ListenerModule::HandleWallButton(mechanicsModule);
        case MainClickID::TOWER_BUTTON:
            return ListenerModule::HandleTowerButton(mechanicsModule);
        case MainClickID::CIRCLE_BUTTON:
            return ListenerModule::HandleUnitButton(mechanicsModule, UserSelect::CIRCLE, MainInputID::CREATE_CIRCLE);
        case MainClickID::TRIANGLE_BUTTON:
            return ListenerModule::HandleUnitButton(mechanicsModule, UserSelect::TRIANGLE, MainInputID::CREATE_TRIANGLE);
        case MainClickID::SQUARE_BUTTON:
            return ListenerModule::HandleUnitButton(mechanicsModule, UserSelect::SQUARE, MainInputID::CREATE_SQUARE);
        case MainClickID::GRID:
            return ListenerModule::HandleGridButton(mechanicsModule, clickInput);
        case MainClickID::CONTINUE:
            return ListenerModule::HandleContinueButton(mechanicsModule);
    }

    return new MainInput(MainInputID::DO_NOTHING, -1, -1, -1);
}

MainClickID ListenerModule::DetermineClicked(ClickInput* clickInput, GameState* gameState, bool isAlive) {
    int clickX = clickInput->GetX();
    int clickY = clickInput->GetY();

    if(!isAlive) {
        if((clickX >= GRAPHICS_UI_MAIN_BUTTONS_CONTINUE[0].first) &&
           (clickX < GRAPHICS_UI_MAIN_BUTTONS_CONTINUE[0].first + GRAPHICS_UI_MAIN_BUTTONS_CONTINUE[1].first) &&
           (clickY >= GRAPHICS_UI_MAIN_BUTTONS_CONTINUE[0].second) &&
           (clickY < GRAPHICS_UI_MAIN_BUTTONS_CONTINUE[0].second + GRAPHICS_UI_MAIN_BUTTONS_CONTINUE[1].second)) {
            return MainClickID::CONTINUE;
        } else {
            return MainClickID::NONE;
        }
    }

    if((clickY >= GRAPHICS_UI_MAIN_BUTTONS_XYS[0].second) && 
       (clickY < GRAPHICS_UI_MAIN_BUTTONS_XYS[0].second + GRAPHICS_UI_MAIN_BUTTONS_SIZE)) {
        int tempX = GRAPHICS_UI_MAIN_BUTTONS_XYS[0].first;
        MainClickID results[] = {MainClickID::WALL_BUTTON,
                                 MainClickID::TOWER_BUTTON,
                                 MainClickID::SQUARE_BUTTON,
                                 MainClickID::TRIANGLE_BUTTON,
                                 MainClickID::CIRCLE_BUTTON};
        for(int i = 0; i < GRAPHICS_UI_MAIN_NUM_BUTTONS; i++) {
            if((clickX >= tempX) && (clickX < tempX + GRAPHICS_UI_MAIN_BUTTONS_SIZE)) {
                return results[i];
            }
            tempX += GRAPHICS_UI_MAIN_BUTTONS_SPREAD + GRAPHICS_UI_MAIN_BUTTONS_SIZE;
        }

        return MainClickID::NONE;
    }

    if((clickX < GRAPHICS_UI_MAIN_ARENA_INSET_X) ||
       (clickX >= CLIENT_SCREEN_WIDTH - GRAPHICS_UI_MAIN_ARENA_INSET_X) ||
       (clickY < GRAPHICS_UI_MAIN_ARENA_INSET_Y) ||
       (clickY >= CLIENT_SCREEN_HEIGHT - GRAPHICS_UI_MAIN_ARENA_INSET_Y)) {
        return MainClickID::NONE;
    }

    //they are on the grid somewhere
    return MainClickID::GRID;
}

MainInput* ListenerModule::HandleWallButton(MechanicsModule* mechanicsModule) {
    if(mechanicsModule->GetUserState()->GetSelected() == UserSelect::WALL_MODE) {
        mechanicsModule->GetUserState()->SetSelected(UserSelect::NONE);
    } else if(mechanicsModule->GetGameState()->GetPlayerMoney(mechanicsModule->GetUserState()->GetThisPlayer()) > MECHANICS_WALL_PRICE) {
        mechanicsModule->GetUserState()->SetSelected(UserSelect::WALL_MODE);
    }

    return new MainInput(MainInputID::DO_NOTHING, -1, -1, -1);
}

MainInput* ListenerModule::HandleTowerButton(MechanicsModule* mechanicsModule) {
    UserState* userState = mechanicsModule->GetUserState();
    if(userState->GetSelected() == UserSelect::TOWER) {
        userState->SetDelay();
        userState->SetSelected(UserSelect::DELETE_TOWER);
        return new MainInput(MainInputID::REMOVE_TOWER, userState->GetSelectedTower()->GetTowerID(), -1, -1);
    } else if(userState->GetSelected() == UserSelect::TOWER_MODE) {
        userState->SetSelected(UserSelect::NONE);
    } else {
        if((mechanicsModule->GetGameState()->GetPlayerMoney(userState->GetThisPlayer()) > MECHANICS_TOWER_PRICE) &&
           (mechanicsModule->GetGameState()->GetPlayer(mechanicsModule->GetUserState()->GetThisPlayer())->GetNumTowers() < MECHANICS_MAX_TOWERS_PER_PLAYER)) {
            userState->SetSelected(UserSelect::TOWER_MODE);
        }
    }

    return new MainInput(MainInputID::DO_NOTHING, -1, -1, -1);
}

MainInput* ListenerModule::HandleUnitButton(MechanicsModule* mechanicsModule, UserSelect newSelect, MainInputID newMainInputId) {
    if(mechanicsModule->GetGameState()->GetPlayerMoney(mechanicsModule->GetUserState()->GetThisPlayer()) > MECHANICS_UNIT_PRICE) {
        mechanicsModule->GetUserState()->SetSelected(newSelect);
        mechanicsModule->GetUserState()->SetDelay();
        return new MainInput(newMainInputId, mechanicsModule->GetUserState()->GetSpawnValues(), -1, -1);
    }
    return new MainInput(MainInputID::DO_NOTHING, -1, -1, -1);
}

MainInput* ListenerModule::HandleContinueButton(MechanicsModule* mechanicsModule) {
    mechanicsModule->GetUserState()->SetSelected(UserSelect::CONTINUE);
    mechanicsModule->GetUserState()->SetDelay();
    return new MainInput(MainInputID::CONTINUE, -1, -1, -1);
}

MainInput* ListenerModule::HandleGridButton(MechanicsModule* mechanicsModule, ClickInput* clickInput) {
    int clickX = clickInput->GetX();
    int clickY = clickInput->GetY();

    int worldX = 0;
    int worldY = 0;
    GameState* gameState = mechanicsModule->GetGameState();
    UserState* userState = mechanicsModule->GetUserState();

    gameState->FindRelativeXY(clickX, clickY, &worldX, &worldY);
    WorldStateID clickedItem = gameState->GetWorldItem(worldX, worldY);

    UserSelect thisSelection = userState->GetSelected();
    if((clickedItem == userState->GetPersonalGoalState()) &&
      (thisSelection == UserSelect::NONE ||
       thisSelection == UserSelect::TOWER ||
       thisSelection == UserSelect::WALL_MODE ||
       thisSelection == UserSelect::TOWER_MODE)) {
        userState->ClickSpawn(worldX, worldY);
        userState->SetSpawnsChanged(true);
        return new MainInput(MainInputID::DO_NOTHING, -1, -1, -1);
    }

    switch(userState->GetSelected()) {
    case UserSelect::NONE:
        if(clickedItem == userState->GetPersonalTowerState()) {
            userState->SetSelected(UserSelect::TOWER);
            Tower* tower = gameState->FindTowerByXY(worldX, worldY);
            if(tower == nullptr) {
                std::cout << "Panic! Tower not found! Will crash!" << std::endl;
                break;
            }

            userState->SetSelectedTower(tower);
        }

        break;
    case UserSelect::TOWER_MODE:
        if(gameState->GetAIModule()->IsFillable(worldX, worldY) &&
           !(gameState->AtMaxTowers(userState->GetThisPlayer()))) {
            userState->SetSelected(UserSelect::PLACE_TOWER);
            userState->SetDelay();
            return new MainInput(MainInputID::BUILD_TOWER, worldX, worldY, -1);
        }

        break;
    case UserSelect::WALL_MODE:
        if(gameState->GetAIModule()->IsFillable(worldX, worldY) &&
          (gameState->GetPlayerMoney(userState->GetThisPlayer()) >= MECHANICS_WALL_PRICE)) {
            userState->SetSelected(UserSelect::PLACE_WALL);
            userState->SetDelay();
            return new MainInput(MainInputID::BUILD_WALL, worldX, worldY, -1);
        }

        break;
    case UserSelect::TOWER:
        if((worldX == userState->GetSelectedTower()->GetX()) &&
           (worldY == userState->GetSelectedTower()->GetY())) {
            userState->SetSelected(UserSelect::NONE);
            break;
        }

        if(userState->GetSelectedTower()->GetWorldStateId() == gameState->GetWorldState()->GetItem(worldX, worldY)) {
            Tower* tower = gameState->FindTowerByXY(worldX, worldY);
            if(tower == nullptr) {
                std::cout << "Panic! Tower not found! Will crash!" << std::endl;
                break;
            }

            userState->SetSelectedTower(tower);
            break;
        }

        if(gameState->WithinSelectedRadius(userState->GetSelectedTower(), worldX, worldY) && !(gameState->GetWorldState()->IsEmpty(worldX, worldY))) {
            userState->SetSelected(UserSelect::TOWER_TARGETS);
            userState->SetDelay();
            return new MainInput(MainInputID::TOWER_TARGETS, userState->GetSelectedTower()->GetTowerID(), worldX, worldY);
        } else {
            userState->SetSelected(UserSelect::NONE);
            break;
        }

        break;
    default:
        break;
    }

    return new MainInput(MainInputID::DO_NOTHING, -1, -1, -1);
}

