#include <iostream>

#include "tasks.h"
#include "..\Graphics\graphics.h"
#include "..\Mechanics\mechanics.h"
#include "..\Mechanics\GameStructure\worldState.h"
#include "..\Mechanics\userState.h"
#include "..\Networking\serverConnect.h"
#include "..\Networking\network.h"
#include "..\config.h"

std::atomic<bool> Tasks::done{false};

Tasks::Tasks() {
    //empty, private, preventing initialization
}

void Tasks::RunStartingTasks(SchedulerModule* schedulerModule) {
    schedulerModule->GetGraphicsModule()->ImportAllMaps();

    Tasks::done = true;
}

void Tasks::RunMainPrepTasks(SchedulerModule* schedulerModule) {
    //get sides and colors of players
    std::pair<int, int>* playersToSides;
    int thisPlayer = 0;

    MechanicsModule* mechanicsModule = schedulerModule->GetMechanicsModule();
    GraphicsModule* graphicsModule = schedulerModule->GetGraphicsModule();
    NetworkingModule* networkingModule = schedulerModule->GetNetworkingModule();
   
    networkingModule->SetMultiplayer(mechanicsModule->IsMultiplayer());
    if(mechanicsModule->IsMultiplayer()) {
        if(playersToSides = networkingModule->ConnectToServer(mechanicsModule->GetNumPlayers(), &thisPlayer); playersToSides == nullptr) {
            networkingModule->SetMultiplayer(false);
            mechanicsModule->SetMultiplayerStatus(false);
            playersToSides = mechanicsModule->FindSPStartingValues(&thisPlayer);
            std::cout << "Multiplayer failed. Starting single player" << std::endl;
        }
    } else {
        playersToSides = mechanicsModule->FindSPStartingValues(&thisPlayer); 
    }

    mechanicsModule->SetThisPlayer(thisPlayer);
    mechanicsModule->SetGameOver(false);

    networkingModule->InitNetworkingFrames(mechanicsModule->GetNumPlayers(), playersToSides, thisPlayer);
    mechanicsModule->InitStates(mechanicsModule->GetNumPlayers(), playersToSides);

    mechanicsModule->GetGameState()->GetAIModule()->GetPathFinder()->InitPathFinder(mechanicsModule->GetNumPlayers(), playersToSides);
    delete playersToSides;

    if(!mechanicsModule->IsMultiplayer()) {
        networkingModule->ConnectBots(mechanicsModule->GetGameState(), mechanicsModule->GetNumPlayers(), playersToSides);
    } else {
        networkingModule->CreateMultiplayerConnection(mechanicsModule->GetNumPlayers(), playersToSides);
    }

    //draw to back buffer
    //background
    graphicsModule->DrawNormalBitmap(graphicsModule->GetBackBuffer(), BitmapID::MAIN_BACKGROUND, 0, 0);

    //money
    graphicsModule->DrawMoney(MECHANICS_PLAYER_STARTING_MONEY);

    //world
    graphicsModule->DrawWorldState(mechanicsModule->GetGameState()->GetWorldState()->GetWorld());
    graphicsModule->DrawGoals(mechanicsModule);

    //buttons
    Tasks::DrawStartingButtons(graphicsModule, mechanicsModule->GetUserState());
    
    Tasks::done = true;
}

void Tasks::DrawStartingButtons(GraphicsModule* graphicsModule, UserState* userState) {
    BitmapID wallId = BitmapID::WALL_BUTTON;
    BitmapID circleId = userState->GetButtonId(UserStateButtonID::CIRCLE_BUTTON);
    BitmapID squareId = userState->GetButtonId(UserStateButtonID::SQUARE_BUTTON);
    BitmapID triangleId = userState->GetButtonId(UserStateButtonID::TRIANGLE_BUTTON);
    BitmapID towerId = userState->GetButtonId(UserStateButtonID::TOWER_BUTTON);

    if(MECHANICS_PLAYER_STARTING_MONEY < MECHANICS_UNIT_PRICE) {
        circleId = BitmapID::UNAVAILABLE_CIRCLE;
        triangleId = BitmapID::UNAVAILABLE_TRIANGLE_BUTTON;
        squareId = BitmapID::UNAVAILABLE_SQUARE_BUTTON;
    }

    if(MECHANICS_PLAYER_STARTING_MONEY < MECHANICS_TOWER_PRICE) {
        towerId = BitmapID::UNAVAILABLE_TOWER;
    }

    if(MECHANICS_PLAYER_STARTING_MONEY < MECHANICS_WALL_PRICE) {
        wallId = BitmapID::WALL_UNAVAILABLE;
    }

    BitmapID ids[GRAPHICS_UI_MAIN_NUM_BUTTONS];
    ids[0] = wallId;
    ids[1] = towerId;
    ids[2] = squareId;
    ids[3] = triangleId;
    ids[4] = circleId;

    for(int i = 0; i < GRAPHICS_UI_MAIN_NUM_BUTTONS; i++) {
        graphicsModule->DrawNormalBitmap(graphicsModule->GetBackBuffer(), ids[i], (GRAPHICS_UI_MAIN_BUTTONS_XYS[i]).first, (GRAPHICS_UI_MAIN_BUTTONS_XYS[i]).second);
    }
}
