#include <iostream>
#include <chrono>
#include <thread>

#include "scheduler.h"
#include "..\Listener\listener.h"
#include "..\Listener\clickInput.h"
#include "..\Listener\mainInput.h"
#include "..\Graphics\graphics.h"
#include "..\Mechanics\mechanics.h"
#include "..\Mechanics\userState.h"
#include "..\Networking\network.h"
#include "..\Networking\frame.h"
#include "..\UI\ui.h"
#include "tasks.h"

using SchedClock = std::chrono::high_resolution_clock;
using Duration = std::chrono::duration<double>;
using TimeStamp = std::chrono::time_point<std::chrono::high_resolution_clock>;

SchedulerModule::SchedulerModule() {

}

SchedulerModule::~SchedulerModule() {
    delete this->graphicsModule;
    delete this->mechanicsModule;
    delete this->networkingModule;
    delete this->uiModule;
    delete this->clickInput;
}

GraphicsModule* SchedulerModule::GetGraphicsModule() {
    return this->graphicsModule;
}

MechanicsModule* SchedulerModule::GetMechanicsModule() {
    return this->mechanicsModule;
}

NetworkingModule* SchedulerModule::GetNetworkingModule() {
    return this->networkingModule;
}

UIModule* SchedulerModule::GetUIModule() {
    return this->uiModule;
}

void SchedulerModule::RunAlphaLoop(HINSTANCE hInstance, int nCmdShow) {
    if(this->graphicsModule == nullptr) {
        this->graphicsModule = new GraphicsModule();
    }
    this->graphicsModule->InitGraphics(hInstance, nCmdShow);

    ClickInput* click = (ClickInput*)GetWindowLongPtr(this->graphicsModule->GetHwnd(), GWLP_USERDATA);
    if(click == nullptr) {
        std::cout << "Click could not be established." << std::endl;
    }
    this->clickInput = click;

    if(this->mechanicsModule == nullptr) {
        this->mechanicsModule = new MechanicsModule();
    }

    if(this->networkingModule == nullptr) {
        this->networkingModule = new NetworkingModule();
        this->mechanicsModule->UpdateNetworkingModule(this->networkingModule);
    }

    if(this->uiModule == nullptr) {
        this->uiModule = new UIModule(this->graphicsModule, this->mechanicsModule);
    }

    this->RunThreadLoop(UIState::MAIN_MENU);
        
    if(!(ListenerModule::IsRunning())) {
        return;
    }

    for(;;) {
        this->RunUILoop();

        if(!(ListenerModule::IsRunning()) || (this->uiModule->GetState() == UIState::QUIT)) {
            return;
        }

        this->RunThreadLoop(UIState::LOADING);

        if(!(ListenerModule::IsRunning())) {
            return;
        }

        this->RunMainLoop();
        this->CleanUpMain();

        if(!(ListenerModule::IsRunning())) {
            return;
        }

    }
}

void SchedulerModule::RunThreadLoop(UIState state) {
    this->graphicsModule->ImportLoadingMaps();
    this->graphicsModule->DrawUIBackground();
    this->graphicsModule->DrawLoadingScreen(0);
    int loadLevel = 0;

    TimeStamp lastTime = SchedClock::now();
    TimeStamp now = SchedClock::now();

    Duration elapsed;
    double accumulator = 0.0;
    const double dt = SCHEDULER_FRAME_RATE_DT;

    Tasks::done = false;
    void (*task)(SchedulerModule*);

    if(state == UIState::MAIN_MENU) {
        task = [](SchedulerModule* schedulerModule) {
            Tasks::RunStartingTasks(schedulerModule);
        };
    } else { // UIState::LOADING
        task = [](SchedulerModule* schedulerModule) {
            Tasks::RunMainPrepTasks(schedulerModule);
        };
    }

    std::thread workerThread(task, this);
    
    while(ListenerModule::IsRunning() && !(Tasks::done)) {
        ListenerModule::CheckInput();
        now = SchedClock::now();
        elapsed = now - lastTime;
        lastTime = now;
        accumulator += elapsed.count();

        while(accumulator >= dt) {
            loadLevel++;

            this->graphicsModule->DrawLoadingScreen((loadLevel / GRAPHICS_UI_LOADING_DELAY) % GRAPHICS_UI_NUM_LOADINGS);

            accumulator -= dt;
        }

        this->graphicsModule->RenderFrame();
    }

    ListenerModule::CheckInput();
    workerThread.join();
}

void SchedulerModule::RunUILoop() {
    this->clickInput->UnsetClicked();
    this->graphicsModule->DrawUI(UIState::MAIN_MENU, -1);

    TimeStamp lastTime = SchedClock::now();
    TimeStamp now = SchedClock::now();

    Duration elapsed;
    double accumulator = 0.0;
    const double dt = SCHEDULER_FRAME_RATE_DT;
    
    while(ListenerModule::IsRunning() && (this->uiModule->GetState() != UIState::LOADING) && (this->uiModule->GetState() != UIState::QUIT)) {
        ListenerModule::CheckInput();
        this->uiModule->HandleInput(this->clickInput);

        now = SchedClock::now();
        elapsed = now - lastTime;
        lastTime = now;
        accumulator += elapsed.count();

        while(accumulator >= dt) {
            this->uiModule->DecrementDelay();

            if(this->uiModule->GetDelay() == 0) {
                this->uiModule->ActOnRender();
                this->uiModule->AdvanceState();
            }

            accumulator -= dt;
        }

        this->graphicsModule->RenderFrame();
    }
}

void SchedulerModule::RunMainLoop() {
    ListenerModule::CheckInput();
    this->clickInput->UnsetClicked();
    this->graphicsModule->PushBackBuffer();

    TimeStamp lastTime = SchedClock::now();
    TimeStamp now = SchedClock::now();

    Duration elapsed;
    double accumulator = 0.0;
    const double dt = SCHEDULER_FRAME_RATE_DT;

    MainInput* currentInput = nullptr;
    Frame* nextFrame = nullptr;

    int frameCount = 0;
   
    bool playing = true;
    while(ListenerModule::IsRunning() && playing) {
        now = SchedClock::now();
        elapsed = now - lastTime;
        lastTime = now;
        accumulator += elapsed.count();
 
        this->networkingModule->ReadConnection();

        while(accumulator >= dt && this->networkingModule->IsFrameFilled(frameCount)) {
            // std::cout << "processing frame" << std::endl;
            ListenerModule::CheckInput();
            currentInput = ListenerModule::TranslateMainInput(this->clickInput, mechanicsModule);

            if(currentInput->type == MainInputID::CONTINUE) {
                this->SetDelay();
            } else if(this->continueDelay >= 0) {
                (this->continueDelay)--;
                if(this->continueDelay == 0) {
                    playing = false;
                    break;
                }
            }

            nextFrame = this->networkingModule->ProcessFrame(currentInput, frameCount);
           
            this->mechanicsModule->UpdateGameState(nextFrame);

            this->graphicsModule->UpdateGraphics(mechanicsModule);

            frameCount++;
            accumulator -= dt;
        }

        this->graphicsModule->RenderFrame();
    }

    this->networkingModule->BreakConnections();

    ListenerModule::CheckInput();
}

void SchedulerModule::SetDelay() {
    if(this->continueDelay < 0) {
        this->continueDelay = NETWORKING_FRAME_DELAY;
    }
}

void SchedulerModule::CleanUpMain() {
    this->networkingModule->BreakConnections();
    this->uiModule->Reset();
    this->graphicsModule->Reset();
}
