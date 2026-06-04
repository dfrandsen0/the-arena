#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include "..\Graphics\graphics.h"
#include "..\Mechanics\mechanics.h"
#include "..\Networking\network.h"
#include "..\Listener\clickInput.h"
#include "..\UI\ui.h"
#include "..\config.h"

class SchedulerModule {
public:
    // enum Mode?
private:
    GraphicsModule* graphicsModule = nullptr;
    MechanicsModule* mechanicsModule = nullptr;
    NetworkingModule* networkingModule = nullptr;
    UIModule* uiModule = nullptr;

    ClickInput* clickInput = nullptr;
    
    int continueDelay = -1;
public:
    SchedulerModule();
    ~SchedulerModule();

    GraphicsModule* GetGraphicsModule();
    MechanicsModule* GetMechanicsModule();
    NetworkingModule* GetNetworkingModule();
    UIModule* GetUIModule();

    void RunAlphaLoop(HINSTANCE hInstance, int nCmdShow);
private:
    void RunThreadLoop(UIState state);
    void RunUILoop();
    void RunMainLoop();

    void SetDelay();
    void CleanUpMain();
};

#endif