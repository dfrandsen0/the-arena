#ifndef LISTENER_H_
#define LISTENER_H_

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "mainInput.h"
#include "clickInput.h"
#include "..\Mechanics\userState.h"
#include "..\Mechanics\gameState.h"
#include "..\Mechanics\mechanics.h"
#include "..\Graphics\graphics.h"

// "static class", essentially. No initiation allowed.
class ListenerModule {
public:
private:
    static MSG msg;
    static bool running; //likely change later, add to user state object
public:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    static void CheckInput();
    static bool IsRunning();

    static MainInput* TranslateMainInput(ClickInput* clickInput, MechanicsModule* mechanicsModule);
private:
    ListenerModule();

    static MainClickID DetermineClicked(ClickInput* clickInput, GameState* gameState, bool isAlive);

    static MainInput* HandleWallButton(MechanicsModule* mechanicsModule);
    static MainInput* HandleTowerButton(MechanicsModule* mechanicsModule);
    static MainInput* HandleUnitButton(MechanicsModule* mechanicsModule, UserSelect newSelect, MainInputID newMainInputId);
    static MainInput* HandleContinueButton(MechanicsModule* mechanicsModule);
    static MainInput* HandleGridButton(MechanicsModule* mechanicsModule, ClickInput* clickInput);
};

#endif