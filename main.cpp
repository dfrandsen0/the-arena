//g++ -Wall -Wconversion -o main.exe main.cpp Listener\listener.cpp Graphics\graphics.cpp Graphics\Direct2D\direct2D.cpp Scheduler\scheduler.cpp Scheduler\tasks.cpp Graphics\BitmapInfo\bitmapInfo.cpp Graphics\BitmapInfo\bitmapObj.cpp Graphics\BitmapInfo\sprites.cpp Mechanics\mechanics.cpp Mechanics\GameObjects\worldState.cpp Mechanics\gameState.cpp UI\ui.cpp Listener\clickInput.cpp -ld2d1 -lgdi32 -luser32
#include <iostream>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>

#include "Scheduler\scheduler.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    //completely failing to randomize.
    // srand(static_cast<unsigned int>(time(nullptr)));

    SchedulerModule schedulerModule;
    schedulerModule.RunAlphaLoop(hInstance, nCmdShow);

    return 0;
}