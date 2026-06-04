#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <utility>
#include <iostream>
#include <cstring>
#include <cstdlib>

#include "graphics.h"
#include "BitmapInfo\bitmapObj.h"
#include "..\Listener\listener.h"
#include "..\Listener\clickInput.h"
#include "..\Mechanics\mechanics.h"
#include "..\Mechanics\GameObjects\unit.h"
#include "..\Mechanics\GameObjects\tower.h"
#include "..\Mechanics\gameState.h"
#include "..\Mechanics\userState.h"
#include "..\Mechanics\GameStructure\worldUpdate.h"
#include "BitmapInfo\sprites.h"
#include "..\config.h"

GraphicsModule::~GraphicsModule() {
    delete this->direct2DModule;
    delete this->bitmapInfoModule;
    delete[] this->pixelBuffer;
}

void GraphicsModule::Reset() {
    this->playerAlive = true;
}

HWND GraphicsModule::GetHwnd() {
    return this->hwnd;
}

UINT32* GraphicsModule::GetPixelBuffer() {
    return this->pixelBuffer;
}

UINT32* GraphicsModule::GetBackBuffer() {
    return this->backBuffer;
}

bool GraphicsModule::InitGraphics(HINSTANCE hInstance, int nCmdShow) {
    if(this->hwnd == NULL) {
        this->CreateWindowModule(hInstance, nCmdShow);
    }

    if(this->direct2DModule == nullptr) {
        this->direct2DModule = new Direct2DModule(this->hwnd);
    }

    if(this->pixelBuffer == nullptr) {
        this->pixelBuffer = new UINT32[CLIENT_SCREEN_HEIGHT * CLIENT_SCREEN_WIDTH];
    }

    if(this->backBuffer == nullptr) {
        this->backBuffer = new UINT32[CLIENT_SCREEN_HEIGHT * CLIENT_SCREEN_WIDTH];
    }

    if(this->bitmapInfoModule == nullptr) {
        this->bitmapInfoModule = new BitmapInfo();
    }

    return true;
}

void GraphicsModule::RenderFrame() {
    this->direct2DModule->RenderFrame(this->pixelBuffer);
}

void GraphicsModule::PushBackBuffer() {
    memcpy(this->pixelBuffer, this->backBuffer, CLIENT_SCREEN_WIDTH * CLIENT_SCREEN_HEIGHT * sizeof(int));
}

void GraphicsModule::ImportLoadingMaps() {
    this->bitmapInfoModule->LoadPPM(BitmapID::UI_BACKGROUND);
    this->bitmapInfoModule->LoadPPM(BitmapID::BOLT);
    this->bitmapInfoModule->LoadPPM(BitmapID::LOADING_EMPTY);
    this->bitmapInfoModule->LoadPPM(BitmapID::LOADING_FILLED);
}

void GraphicsModule::ImportAllMaps() {
    for(int i = 0; i < GRAPHICS_NUM_BITMAPS; i++) {
        this->bitmapInfoModule->LoadPPM(static_cast<BitmapID>(i));
    }
}

void GraphicsModule::DrawPrettyColors(int offset) {
    UINT8 r, g, b;
    for(int y = 0; y < CLIENT_SCREEN_HEIGHT; y++) {
        for (int x = 0; x < CLIENT_SCREEN_WIDTH; x++) {
            r = (UINT8)((x + offset) % 256);
            g = (UINT8)((y + offset) % 256);
            b = (UINT8)((x + y + offset) % 256);
            this->pixelBuffer[y * CLIENT_SCREEN_WIDTH + x] = 0xFF000000 | (r << 16) | (g << 8) | b;
        }
    }
}

void GraphicsModule::DrawUIBackground() {
    this->DrawNormalBitmap(this->pixelBuffer, BitmapID::UI_BACKGROUND, 0, 0);

    for(int i = 0; i < GRAPHICS_UI_NUM_BOLTS; i++) {
        this->DrawNormalBitmap(this->pixelBuffer, BitmapID::BOLT, (GRAPHICS_UI_BOLTS_XYS[i]).first, (GRAPHICS_UI_BOLTS_XYS[i]).second);
    }
}

void GraphicsModule::DrawLoadingScreen(int loadLevel) {
    for(int i = 0; i < GRAPHICS_UI_NUM_LOADINGS; i++) {
        if(i == loadLevel) {
            this->DrawNormalBitmap(this->pixelBuffer, BitmapID::LOADING_FILLED, (GRAPHICS_UI_LOADING_XYS[i]).first, (GRAPHICS_UI_LOADING_XYS[i]).second);
        } else {
            this->DrawNormalBitmap(this->pixelBuffer, BitmapID::LOADING_EMPTY, (GRAPHICS_UI_LOADING_XYS[i]).first, (GRAPHICS_UI_LOADING_XYS[i]).second);
        }
    }
}

void GraphicsModule::DrawUI(UIState state, int pressedIndexFromTop) {
    this->DrawUIBackground();

    //flavor: add ui/human bitmaps on top of buttons
    switch(state) {
        case UIState::MAIN_MENU:
            for(int i = 0; i < 2; i++) {
                if(i == pressedIndexFromTop) {
                    this->DrawNormalBitmap(this->pixelBuffer, BitmapID::UI_BUTTON_PRESSED_TEMPLATE, UI_BUTTONS_XYS[0][i].first, UI_BUTTONS_XYS[0][i].second);
                    this->DrawButtonFlavors(i, true, state, UI_BUTTONS_XYS[0][i].first, UI_BUTTONS_XYS[0][i].second);
                } else {
                    this->DrawNormalBitmap(this->pixelBuffer, BitmapID::UI_BUTTON_TEMPLATE, UI_BUTTONS_XYS[0][i].first, UI_BUTTONS_XYS[0][i].second);
                    this->DrawButtonFlavors(i, false, state, UI_BUTTONS_XYS[0][i].first, UI_BUTTONS_XYS[0][i].second);
                }
            }

            if(pressedIndexFromTop == 2) {
                this->DrawNormalBitmap(this->pixelBuffer, BitmapID::UI_X_PRESSED, UI_BUTTONS_XYS[0][2].first, UI_BUTTONS_XYS[0][2].second);
            } else {
                this->DrawNormalBitmap(this->pixelBuffer, BitmapID::UI_X_BUTTON, UI_BUTTONS_XYS[0][2].first, UI_BUTTONS_XYS[0][2].second);
            }
            break;
        case UIState::SINGLE_PLAYER:
        case UIState::MULTI_PLAYER:
            pressedIndexFromTop -= 3;
            for(int i = 0; i < 3; i++) {
                if(i == pressedIndexFromTop) {
                    this->DrawNormalBitmap(this->pixelBuffer, BitmapID::UI_BUTTON_PRESSED_TEMPLATE, UI_BUTTONS_XYS[1][i].first, UI_BUTTONS_XYS[1][i].second);
                    this->DrawButtonFlavors(i + 2, true, state, UI_BUTTONS_XYS[1][i].first, UI_BUTTONS_XYS[1][i].second);
                } else {
                    this->DrawNormalBitmap(this->pixelBuffer, BitmapID::UI_BUTTON_TEMPLATE, UI_BUTTONS_XYS[1][i].first, UI_BUTTONS_XYS[1][i].second);
                    this->DrawButtonFlavors(i + 2, false, state, UI_BUTTONS_XYS[1][i].first, UI_BUTTONS_XYS[1][i].second);
                }
            }

            if(pressedIndexFromTop == 3) {
                this->DrawNormalBitmap(this->pixelBuffer, BitmapID::UI_BACK_PRESSED, UI_BUTTONS_XYS[1][3].first, UI_BUTTONS_XYS[1][3].second);
            } else {
                this->DrawNormalBitmap(this->pixelBuffer, BitmapID::UI_BACK_BUTTON, UI_BUTTONS_XYS[1][3].first, UI_BUTTONS_XYS[1][3].second);
            }
            break;
        case UIState::LOADING:
            this->DrawLoadingScreen(0);
            break;
        case UIState::QUIT:
            break;
        default:
            std::cout << "Strange state passed to GraphicsModule::DrawUI." << std::endl;
    }
}

void GraphicsModule::DrawButtonFlavors(int index, bool pressed, UIState state, int baseX, int baseY) {
    BitmapID human = BitmapID::UI_HUMAN_BUTTON;
    BitmapID robot = BitmapID::UI_ROBOT;

    if(pressed) {
        human = BitmapID::UI_HUMAN_PRESSED;
        robot = BitmapID::UI_ROBOT_PRESSED;
    }

    BitmapID ids[4];
    BitmapID secondary;
    if(index == 0 || state == UIState::SINGLE_PLAYER) {
        secondary = robot;
    } else if(index == 1 || state == UIState::MULTI_PLAYER) {
        secondary = human;
    }

    ids[0] = human;
    for(int i = 1; i < 4; i++) {
        ids[i] = secondary;
    }

    switch(index) {
        case 0: //singleplayer
            for(int i = 0; i < 4; i++) {
                this->DrawNormalBitmap(this->pixelBuffer, ids[i], UI_BUTTONS_FLAVORS[2][i].first + baseX, UI_BUTTONS_FLAVORS[2][i].second + baseY);
            }
            break;
        case 1: //multiplayer
            for(int i = 0; i < 4; i++) {
                this->DrawNormalBitmap(this->pixelBuffer, ids[i], UI_BUTTONS_FLAVORS[2][i].first + baseX, UI_BUTTONS_FLAVORS[2][i].second + baseY);
            }
            break;
        case 2: //2 player
            for(int i = 0; i < 2; i++) {
                this->DrawNormalBitmap(this->pixelBuffer, ids[i], UI_BUTTONS_FLAVORS[0][i].first + baseX, UI_BUTTONS_FLAVORS[0][i].second + baseY);
            }
            break;
        case 3: //3 player
            for(int i = 0; i < 3; i++) {
                this->DrawNormalBitmap(this->pixelBuffer, ids[i], UI_BUTTONS_FLAVORS[1][i].first + baseX, UI_BUTTONS_FLAVORS[1][i].second + baseY);
            }
            break;
        case 4: //4 player
            for(int i = 0; i < 4; i++) {
                this->DrawNormalBitmap(this->pixelBuffer, ids[i], UI_BUTTONS_FLAVORS[2][i].first + baseX, UI_BUTTONS_FLAVORS[2][i].second + baseY);
            }
            break;
        default:
            break;
    }
}

void GraphicsModule::DrawWorldState(WorldStateID* world) {
    for(int y = 0; y < MECHANICS_WORLD_STATE_HEIGHT; y++) {
        for(int x = 0; x < MECHANICS_WORLD_STATE_WIDTH; x++) {
            this->DrawNormalBitmap(this->backBuffer, Sprites::WorldStateIDToBitmapID(static_cast<WorldStateID>(world[(y * MECHANICS_WORLD_STATE_WIDTH) + x])), 24 * (x + 1), 24 * (y + 1));
        }
    }
}

void GraphicsModule::DrawMoney(int moneyValue) {
    if(moneyValue > GRAPHICS_UI_NUM_MONEY) {
        moneyValue = moneyValue / MECHANICS_MONEY_AMPLIFIER;
    }

    int i = 0;
    for(; i < moneyValue; i++) {
        this->DrawNormalBitmap(this->GetBackBuffer(), BitmapID::MONEY_FILLED, (GRAPHICS_UI_MONEY_XYS[i]).first, (GRAPHICS_UI_MONEY_XYS[i]).second);
    }
    
    for(; i < GRAPHICS_UI_NUM_MONEY; i++) {
        this->DrawNormalBitmap(this->GetBackBuffer(), BitmapID::MONEY_EMPTY, (GRAPHICS_UI_MONEY_XYS[i]).first, (GRAPHICS_UI_MONEY_XYS[i]).second);
    }
}

void GraphicsModule::DrawGoals(MechanicsModule* mechanicsModule) {
    UserState* userState = mechanicsModule->GetUserState();
    GameState* gameState = mechanicsModule->GetGameState();

    int spawns = userState->GetSpawnValues();
    int spawnChecks[MECHANICS_NUM_GOALS] = {1, 2, 4};
    int paintX, paintY;
    Goal* tempGoal;

    for(int i = 1; i <= MECHANICS_NUM_GOALS; i++) {
        tempGoal = gameState->GetGoal(userState->GetThisPlayer(), i);
        if(tempGoal == nullptr) {
            continue;
        }
        
        this->CellToScreen(tempGoal->GetX(), tempGoal->GetY(), &paintX, &paintY);
        if(spawns & spawnChecks[i - 1]) {
            this->DrawNormalBitmap(this->backBuffer, userState->GetPersonalGoalSelectedId(), paintX, paintY);
        } else {
            this->DrawNormalBitmap(this->backBuffer, userState->GetPersonalGoalBitmapId(), paintX, paintY);
        }
    }
}

void GraphicsModule::DrawNormalBitmap(UINT32* buffer, BitmapID bitmapID, int x, int y) {
    BitmapObj* bitmapObj = this->bitmapInfoModule->GetBitmap(bitmapID);
    int width = bitmapObj->GetWidth();
    int height = bitmapObj->GetHeight();
    UINT32* sourcePixels = bitmapObj->GetPixels();
    
    int currY = y * CLIENT_SCREEN_WIDTH;
    int currYSrc = 0;

    int trueWidth = width * sizeof(int);

    for(int row = 0; row < height; row++) {
        memcpy(buffer + currY + x, sourcePixels + currYSrc, trueWidth);
        currY += CLIENT_SCREEN_WIDTH;
        currYSrc += width;
    }
}

void GraphicsModule::UpdateGraphics(MechanicsModule* mechanicsModule) {
    UserState* userState = mechanicsModule->GetUserState();
    GameState* gameState = mechanicsModule->GetGameState();

    int moneyDiff = gameState->GetMoneyDifference(userState->GetThisPlayer());
    if(userState->ThisPlayerIsAlive() && moneyDiff) {
        this->DrawMoney(moneyDiff);
        int playerMoney = gameState->GetPlayerMoney(userState->GetThisPlayer());
        if(playerMoney < MECHANICS_UNIT_PRICE) {
            this->DrawNormalBitmap(this->backBuffer, BitmapID::UNAVAILABLE_SQUARE_BUTTON, GRAPHICS_UI_MAIN_BUTTONS_XYS[2].first, GRAPHICS_UI_MAIN_BUTTONS_XYS[2].second);
            this->DrawNormalBitmap(this->backBuffer, BitmapID::UNAVAILABLE_TRIANGLE_BUTTON, GRAPHICS_UI_MAIN_BUTTONS_XYS[3].first, GRAPHICS_UI_MAIN_BUTTONS_XYS[3].second);
            this->DrawNormalBitmap(this->backBuffer, BitmapID::UNAVAILABLE_CIRCLE, GRAPHICS_UI_MAIN_BUTTONS_XYS[4].first, GRAPHICS_UI_MAIN_BUTTONS_XYS[4].second);
        } else {
            this->DrawNormalBitmap(this->backBuffer, userState->GetButtonId(UserStateButtonID::SQUARE_BUTTON), GRAPHICS_UI_MAIN_BUTTONS_XYS[2].first, GRAPHICS_UI_MAIN_BUTTONS_XYS[2].second);
            this->DrawNormalBitmap(this->backBuffer, userState->GetButtonId(UserStateButtonID::TRIANGLE_BUTTON), GRAPHICS_UI_MAIN_BUTTONS_XYS[3].first, GRAPHICS_UI_MAIN_BUTTONS_XYS[3].second);
            this->DrawNormalBitmap(this->backBuffer, userState->GetButtonId(UserStateButtonID::CIRCLE_BUTTON), GRAPHICS_UI_MAIN_BUTTONS_XYS[4].first, GRAPHICS_UI_MAIN_BUTTONS_XYS[4].second);
        }

        if(playerMoney < MECHANICS_TOWER_PRICE || gameState->GetPlayer(userState->GetThisPlayer())->GetNumTowers() == MECHANICS_MAX_TOWERS_PER_PLAYER) {
            this->DrawNormalBitmap(this->backBuffer, BitmapID::UNAVAILABLE_TOWER, GRAPHICS_UI_MAIN_BUTTONS_XYS[1].first, GRAPHICS_UI_MAIN_BUTTONS_XYS[1].second);
        } else {
            this->DrawNormalBitmap(this->backBuffer, userState->GetButtonId(UserStateButtonID::TOWER_BUTTON), GRAPHICS_UI_MAIN_BUTTONS_XYS[1].first, GRAPHICS_UI_MAIN_BUTTONS_XYS[1].second);
        }

        if(playerMoney < MECHANICS_WALL_PRICE) {
            this->DrawNormalBitmap(this->backBuffer, BitmapID::WALL_UNAVAILABLE, GRAPHICS_UI_MAIN_BUTTONS_XYS[0].first, GRAPHICS_UI_MAIN_BUTTONS_XYS[0].second);
        } else {
            this->DrawNormalBitmap(this->backBuffer, BitmapID::WALL_BUTTON, GRAPHICS_UI_MAIN_BUTTONS_XYS[0].first, GRAPHICS_UI_MAIN_BUTTONS_XYS[0].second);
        }
    }

    //feels like "update game state", doesn't it...
    gameState->UpdateLastMoneys();

    if(userState->HaveSpawnsChanged()) {        
        this->DrawGoals(mechanicsModule);
        userState->SetSpawnsChanged(false);
    }

    int updateCount = gameState->GetWorldState()->GetWorldUpdateCount();
    if(updateCount > 0) {
        WorldUpdate** updates = gameState->GetWorldState()->GetWorldUpdates();
        int paintX, paintY;
        for(int i = 0; i < updateCount; i++) {
            this->CellToScreen(updates[i]->worldX, updates[i]->worldY, &paintX, &paintY);
            this->DrawNormalBitmap(this->backBuffer, Sprites::WorldStateIDToBitmapID(updates[i]->stateId), paintX, paintY);
            if(updates[i]->stateId == WorldStateID::DEAD_GOAL) {
                this->UserStateCheck(userState);
            }
            delete updates[i];
        }
        gameState->GetWorldState()->ResetWorldUpdateCount();
    }

    //draw background
    this->PushBackBuffer();

    switch(userState->GetSelected()) {
        case UserSelect::CIRCLE:
            this->DrawNormalBitmap(this->pixelBuffer, userState->GetButtonId(UserStateButtonID::CIRCLE_PRESSED), GRAPHICS_UI_MAIN_BUTTONS_XYS[4].first, GRAPHICS_UI_MAIN_BUTTONS_XYS[4].second);
            break;
        case UserSelect::SQUARE:
            this->DrawNormalBitmap(this->pixelBuffer, userState->GetButtonId(UserStateButtonID::SQUARE_PRESSED), GRAPHICS_UI_MAIN_BUTTONS_XYS[2].first, GRAPHICS_UI_MAIN_BUTTONS_XYS[2].second);
            break;
        case UserSelect::TRIANGLE:
            this->DrawNormalBitmap(this->pixelBuffer, userState->GetButtonId(UserStateButtonID::TRIANGLE_PRESSED), GRAPHICS_UI_MAIN_BUTTONS_XYS[3].first, GRAPHICS_UI_MAIN_BUTTONS_XYS[3].second);
            break;
        case UserSelect::TOWER_MODE:
        case UserSelect::PLACE_TOWER:
            this->DrawNormalBitmap(this->pixelBuffer, userState->GetButtonId(UserStateButtonID::TOWER_PRESSED), GRAPHICS_UI_MAIN_BUTTONS_XYS[1].first, GRAPHICS_UI_MAIN_BUTTONS_XYS[1].second);
            break;
        case UserSelect::WALL_MODE:
        case UserSelect::PLACE_WALL:
            this->DrawNormalBitmap(this->pixelBuffer, BitmapID::WALL_PRESSED, GRAPHICS_UI_MAIN_BUTTONS_XYS[0].first, GRAPHICS_UI_MAIN_BUTTONS_XYS[0].second);
            break;
        case UserSelect::TOWER:
            //slight inefficiency here
            this->DrawNormalBitmap(this->pixelBuffer, BitmapID::MAIN_X_BUTTON, GRAPHICS_UI_MAIN_BUTTONS_XYS[1].first, GRAPHICS_UI_MAIN_BUTTONS_XYS[1].second);
            break;
        case UserSelect::DELETE_TOWER:
            this->DrawNormalBitmap(this->pixelBuffer, BitmapID::MAIN_X_PRESSED, GRAPHICS_UI_MAIN_BUTTONS_XYS[1].first, GRAPHICS_UI_MAIN_BUTTONS_XYS[1].second);
            if((gameState->GetPlayer(userState->GetThisPlayer())->GetNumTowers() == MECHANICS_MAX_TOWERS_PER_PLAYER) ||
               (gameState->GetPlayer(userState->GetThisPlayer())->GetMoney() < MECHANICS_TOWER_PRICE)) {
                this->DrawNormalBitmap(this->backBuffer, BitmapID::UNAVAILABLE_TOWER, GRAPHICS_UI_MAIN_BUTTONS_XYS[1].first, GRAPHICS_UI_MAIN_BUTTONS_XYS[1].second);
            } else {
                this->DrawNormalBitmap(this->backBuffer, userState->GetButtonId(UserStateButtonID::TOWER_BUTTON), GRAPHICS_UI_MAIN_BUTTONS_XYS[1].first, GRAPHICS_UI_MAIN_BUTTONS_XYS[1].second);
            }
            break;
        case UserSelect::CONTINUE:
            this->DrawNormalBitmap(this->pixelBuffer, BitmapID::UI_CONTINUE_PRESSED, GRAPHICS_UI_MAIN_BUTTONS_CONTINUE[0].first, GRAPHICS_UI_MAIN_BUTTONS_CONTINUE[0].second);
            break;
        default: //tower targets, none
            break;
    }

    //draw units
    for(int i = 0; i < gameState->GetLastUnit(); i++) {
        this->DrawUnit(gameState->GetUnit(i));
    }

    //drawing towers after units puts bullets overtop units
    //draw tower pointers, draw bullets
    Tower** towers = gameState->GetAllTowers();
    for(int i = 0; i < gameState->towersSize; i++) {
        if(towers[i] == nullptr) {
            continue;
        }

        this->DrawTowerLaser(towers[i]);
    }

    if((userState->GetSelected() == UserSelect::WALL_MODE) ||
       (userState->GetSelected() == UserSelect::TOWER_MODE)) {
        this->DrawBitmapUnderCursor(mechanicsModule);
    }

    if(userState->GetSelected() == UserSelect::TOWER) {
        int worldX = userState->GetSelectedTower()->GetX();
        int worldY = userState->GetSelectedTower()->GetY();
        int paintX, paintY;
        this->CellToScreen(worldX, worldY, &paintX, &paintY);
        this->DrawTowerRadius(paintX, paintY, true);
    }
}

void GraphicsModule::DrawUnit(Unit* unit) {
    switch(unit->GetType()) {
        case UnitType::CIRCLE:
            this->DrawCircle(unit);
            break;
        case UnitType::SQUARE:
            this->DrawSquare(unit);
            break;
        case UnitType::TRIANGLE:
            this->DrawTriangle(unit);
            break;
    }
}

std::pair<int, int> GraphicsModule::GetBitmapSize(BitmapID id) {
    BitmapObj* obj = this->bitmapInfoModule->GetBitmap(id);
    return {obj->GetWidth(), obj->GetHeight()};
}

bool GraphicsModule::WithinButton(int clickX, int clickY, int buttonX, int buttonY, BitmapID id) {
    if(clickX < buttonX || clickY < buttonY) {
        return false;
    }

    BitmapObj* obj = this->bitmapInfoModule->GetBitmap(id);

    if(clickX >= buttonX + obj->GetWidth()) {
        return false;
    }

    if(clickY >= buttonY + obj->GetHeight()) {
        return false;
    }

    return true;
}

bool GraphicsModule::CreateWindowModule(HINSTANCE hInstance, int nCmdShow) {
    const wchar_t WINDOW_CLASS_NAME[] = L"Arena Window Class";

    WNDCLASSW winClass = {};
    winClass.lpfnWndProc = ListenerModule::WndProc;
    winClass.hInstance = hInstance;
    winClass.lpszClassName = WINDOW_CLASS_NAME;

    RegisterClassW(&winClass);

    RECT rect = {0, 0, 648, 768};
    AdjustWindowRectEx(&rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE, 0);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    this->hwnd = CreateWindowExW(
        0,
        WINDOW_CLASS_NAME,
        L"The Arena",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        (screenWidth - (rect.right - rect.left)) / 2, (screenHeight - (rect.bottom - rect.top)) / 2, rect.right - rect.left, rect.bottom - rect.top,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if(hwnd == NULL) {
        std::cout << "Failed to create window handle." << std::endl;
        return 1;
    }

    ClickInput* clickInput = new ClickInput();
    SetWindowLongPtr(this->hwnd, GWLP_USERDATA, (LONG_PTR)clickInput);

    ShowWindow(this->hwnd, nCmdShow);
    
    return 0;
}

void GraphicsModule::CellToScreen(int cellX, int cellY, int* clientX, int* clientY) {
    *clientX = cellX * GRAPHICS_UI_MAIN_ARENA_CELL_SIZE + GRAPHICS_UI_MAIN_ARENA_INSET_X;
    *clientY = cellY * GRAPHICS_UI_MAIN_ARENA_CELL_SIZE + GRAPHICS_UI_MAIN_ARENA_INSET_Y;
}

void GraphicsModule::WorldToScreen(int worldX, int worldY, int* clientX, int* clientY) {
    *clientX = worldX + GRAPHICS_UI_MAIN_ARENA_INSET_X;
    *clientY = worldY + GRAPHICS_UI_MAIN_ARENA_INSET_Y;
}

void GraphicsModule::ItemCenterScreen(int itemX, int itemY, int* clientX, int* clientY) {
    *clientX = itemX * GRAPHICS_UI_MAIN_ARENA_CELL_SIZE + GRAPHICS_UI_MAIN_ARENA_HALF_CELL_SIZE + GRAPHICS_UI_MAIN_ARENA_INSET_X;
    *clientY = itemY * GRAPHICS_UI_MAIN_ARENA_CELL_SIZE + GRAPHICS_UI_MAIN_ARENA_HALF_CELL_SIZE + GRAPHICS_UI_MAIN_ARENA_INSET_Y;
}

void GraphicsModule::DrawSquare(Unit* unit) {
    BitmapID bitmapId;

    switch(unit->GetColor()) {
        case PlayerColor::BLUE:
            bitmapId = BitmapID::BLUE_SQUARE_UNIT;
            break;
        case PlayerColor::RED:
            bitmapId = BitmapID::RED_SQUARE_UNIT;
            break;
        case PlayerColor::GREEN:
            bitmapId = BitmapID::GREEN_SQUARE_UNIT;
            break;
        case PlayerColor::YELLOW:
            bitmapId = BitmapID::YELLOW_SQUARE_UNIT;
            break;
    }

    this->DrawNormalBitmap(this->pixelBuffer, bitmapId, unit->GetPixelX(), unit->GetPixelY());
}

void GraphicsModule::DrawTriangle(Unit* unit) {
    BitmapID bitmapId;

    switch(unit->GetColor()) {
        case PlayerColor::BLUE:
            bitmapId = BitmapID::BLUE_TRIANGLE_UNIT;
            break;
        case PlayerColor::RED:
            bitmapId = BitmapID::RED_TRIANGLE_UNIT;
            break;
        case PlayerColor::GREEN:
            bitmapId = BitmapID::GREEN_TRIANGLE_UNIT;
            break;
        case PlayerColor::YELLOW:
            bitmapId = BitmapID::YELLOW_TRIANGLE_UNIT;
            break;
    }

    BitmapObj* bitmapObj = this->bitmapInfoModule->GetBitmap(bitmapId);
    int height = bitmapObj->GetHeight();
    int width = bitmapObj->GetWidth();
    UINT32* sourcePixels = bitmapObj->GetPixels();

    int x = unit->GetPixelX();
    int y = unit->GetPixelY();

    int halfWidth = width / 2;

    this->pixelBuffer[y * CLIENT_SCREEN_WIDTH + x + halfWidth] = sourcePixels[halfWidth + x];

    int currY = (y + 1) * CLIENT_SCREEN_WIDTH;
    int currYSrc = width;
    int currWidth = 3;
    int offset = 1;

    for(int row = 1; row < height; row++) {
        memcpy(this->pixelBuffer + currY + x + (halfWidth - offset), sourcePixels + currYSrc + (halfWidth - offset), currWidth * sizeof(int));
        currY += CLIENT_SCREEN_WIDTH;
        currYSrc += width;
        if(row % 2 == 0) {
            currWidth += 2;
            offset++;
        }
    }
}

void GraphicsModule::DrawCircle(Unit* unit) {
    BitmapID bitmapId;

    switch(unit->GetColor()) {
        case PlayerColor::BLUE:
            bitmapId = BitmapID::BLUE_CIRCLE_UNIT;
            break;
        case PlayerColor::RED:
            bitmapId = BitmapID::RED_CIRCLE_UNIT;
            break;
        case PlayerColor::GREEN:
            bitmapId = BitmapID::GREEN_CIRCLE_UNIT;
            break;
        case PlayerColor::YELLOW:
            bitmapId = BitmapID::YELLOW_CIRCLE_UNIT;
            break;
    }

    BitmapObj* bitmapObj = this->bitmapInfoModule->GetBitmap(bitmapId);
    int height = bitmapObj->GetHeight();
    int width = bitmapObj->GetWidth();
    UINT32* sourcePixels = bitmapObj->GetPixels();

    int x = unit->GetPixelX();
    int y = unit->GetPixelY();

    int currY = y * CLIENT_SCREEN_WIDTH;
    int currYSrc = 0;

    for(int row = 0; row < height; row++) {
        memcpy(this->pixelBuffer + currY + x + GRAPHICS_BITMAP_UNIT_CIRCLE_OFFSET[row], sourcePixels + currYSrc + GRAPHICS_BITMAP_UNIT_CIRCLE_OFFSET[row], GRAPHICS_BITMAP_UNIT_CIRCLE_ROWS[row] * sizeof(int));
        currY += CLIENT_SCREEN_WIDTH;
        currYSrc += width;
    }
}

void GraphicsModule::DrawTowerLaser(Tower* tower) {
    if(tower->IsWaiting() || tower->GetTargetX() == -1) {
        return;
    }

    int endX, endY;
    if(!tower->IsTargeting()) {
        this->WorldToScreen(tower->GetTargetX(), tower->GetTargetY(), &endX, &endY);
    } else {
        this->ItemCenterScreen(tower->GetTargetX(), tower->GetTargetY(), &endX, &endY);
    }

    this->DrawLine(tower->GetScreenX(), tower->GetScreenY(), endX, endY, GRAPHICS_TOWER_LASER_COLORS[static_cast<int>(tower->GetColor()) - 1]);
}

void GraphicsModule::DrawBitmapUnderCursor(MechanicsModule* mechanicsModule) {
    POINT currPos;
    GetCursorPos(&currPos);
    ScreenToClient(this->hwnd, &currPos);

    //within arena
    if((currPos.x >= GRAPHICS_UI_MAIN_ARENA_INSET_X + GRAPHICS_UI_MAIN_ARENA_CELL_SIZE) &&
        (currPos.y >= GRAPHICS_UI_MAIN_ARENA_INSET_Y + GRAPHICS_UI_MAIN_ARENA_CELL_SIZE) &&
        (currPos.x < GRAPHICS_UI_MAIN_ARENA_INSET_X + GRAPHICS_UI_MAIN_ARENA_CELL_SIZE * (MECHANICS_WORLD_STATE_WIDTH - 1)) &&
        (currPos.y < GRAPHICS_UI_MAIN_ARENA_INSET_Y + GRAPHICS_UI_MAIN_ARENA_CELL_SIZE * (MECHANICS_WORLD_STATE_HEIGHT - 1))) {

        int paintX = currPos.x - (currPos.x % GRAPHICS_UI_MAIN_ARENA_CELL_SIZE);
        int paintY = currPos.y - (currPos.y % GRAPHICS_UI_MAIN_ARENA_CELL_SIZE);

        if(mechanicsModule->GetUserState()->GetSelected() == UserSelect::WALL_MODE) {
            if(mechanicsModule->GetGameState()->GetPlayerMoney(mechanicsModule->GetThisPlayer()) < MECHANICS_WALL_PRICE) {
                this->DrawNormalBitmap(this->pixelBuffer, BitmapID::WOOD_WALL_CURSOR_UNAVAILABLE, paintX, paintY);
            } else if(mechanicsModule->GetGameState()->GetAIModule()->IsFillable((paintX / GRAPHICS_UI_MAIN_ARENA_CELL_SIZE) - 1, (paintY / GRAPHICS_UI_MAIN_ARENA_CELL_SIZE) - 1)) {
                this->DrawNormalBitmap(this->pixelBuffer, BitmapID::WOOD_WALL, paintX, paintY);
            } else {
                this->DrawNormalBitmap(this->pixelBuffer, BitmapID::WOOD_WALL_UNPLACE, paintX, paintY);
            }
        } else {
            this->DrawNormalBitmap(this->pixelBuffer, mechanicsModule->GetUserState()->GetPersonalTowerBitmapId(), paintX, paintY);
            if(mechanicsModule->GetGameState()->GetAIModule()->IsFillable((paintX / GRAPHICS_UI_MAIN_ARENA_CELL_SIZE) - 1, (paintY / GRAPHICS_UI_MAIN_ARENA_CELL_SIZE) - 1)) {
                this->DrawTowerRadius(paintX, paintY, true);
            } else {
                this->DrawTowerRadius(paintX, paintY, false);
            }
        }
    }
}

void GraphicsModule::DrawTowerRadius(int paintX, int paintY, bool fillable) {
    int cx = paintX - 1 + GRAPHICS_UI_MAIN_ARENA_CELL_SIZE / 2;
    int cy = paintY - 1 + GRAPHICS_UI_MAIN_ARENA_CELL_SIZE / 2;

    int radius = GRAPHICS_TOWER_RADIUS * GRAPHICS_UI_MAIN_ARENA_CELL_SIZE;
    int innerRadius = radius - GRAPHICS_TOWER_RADIUS_BORDER_WIDTH;

    double srcR = 0.0;
    double srcG = 88.0;
    double srcB = 255.0;
    int borderColor = GRAPHICS_TOWER_RADIUS_BORDER_FILLABLE_COLOR;

    const float oppositeAlpha = 1 - GRAPHICS_TOWER_RADIUS_ALPHA;

    if(!fillable) {
        srcR = 255.0;
        srcG = 20.0;
        srcB = 0.0;
        borderColor = GRAPHICS_TOWER_RADIUS_BORDER_UNFILLABLE_COLOR;
    }

    int leftBorder = GRAPHICS_UI_MAIN_ARENA_INSET_X + GRAPHICS_UI_MAIN_ARENA_CELL_SIZE;
    int rightBorder = GRAPHICS_UI_MAIN_ARENA_INSET_X + (MECHANICS_WORLD_STATE_WIDTH - 1) * GRAPHICS_UI_MAIN_ARENA_CELL_SIZE;
    int topBorder = GRAPHICS_UI_MAIN_ARENA_INSET_Y + GRAPHICS_UI_MAIN_ARENA_CELL_SIZE;
    int bottomBorder = GRAPHICS_UI_MAIN_ARENA_INSET_Y + (MECHANICS_WORLD_STATE_HEIGHT - 1) * GRAPHICS_UI_MAIN_ARENA_CELL_SIZE;

    srcR = srcR * GRAPHICS_TOWER_RADIUS_ALPHA;
    srcG = srcG * GRAPHICS_TOWER_RADIUS_ALPHA;
    srcB = srcB * GRAPHICS_TOWER_RADIUS_ALPHA;

    UINT32 dst;
    UINT8 dstB, dstG, dstR;
    UINT8 outR, outG, outB;
    int dx, dy;

    for (int y = cy - radius; y <= cy + radius; y++) {
        for (int x = cx - radius; x <= cx + radius; x++) {
            dx = x - cx;
            dy = y - cy;
            if (dx*dx + dy*dy <= radius*radius) {
                if (x >= leftBorder && x < rightBorder && y >= topBorder && y < bottomBorder) {
                    if(dx*dx + dy*dy > innerRadius*innerRadius) {
                        this->pixelBuffer[y * CLIENT_SCREEN_WIDTH + x] = borderColor;
                        continue;
                    }

                    //extract dest pixel (BGRA order in memory)
                    dst = this->pixelBuffer[y * CLIENT_SCREEN_WIDTH + x];

                    dstB = dst & 0xFF;
                    dstG = (dst >> 8) & 0xFF;
                    dstR = (dst >> 16) & 0xFF;

                    //blend src and dst
                    outR = (UINT8)(srcR + dstR * oppositeAlpha);
                    outG = (UINT8)(srcG + dstG * oppositeAlpha);
                    outB = (UINT8)(srcB + dstB * oppositeAlpha);

                    this->pixelBuffer[y * CLIENT_SCREEN_WIDTH + x] = 0xFF000000 | (outR << 16) | (outG << 8) | outB;
                }
            }
        }
    }
}

void GraphicsModule::DrawLine(int startX, int startY, int endX, int endY, int color) {
    int dx = abs(endX - startX);
    int dy = abs(endY - startY);
    int xChange = 1;
    int yChange = 1;

    if(startX >= endX) {
        xChange = -1;
    }

    if(startY >= endY) {
        yChange = -1;
    }

    bool steep = false;
    if(dy > dx) {
        steep = true;
        std::swap(startX, startY);
        std::swap(endX, endY);
        std::swap(dx, dy);
        std::swap(xChange, yChange);
    }

    int err = dx / 2;
    int x = startX;
    int y = startY;

    while(x != endX + xChange) {
        if (steep) {
            this->Draw2x2Square(y, x, color);
        } else {
            this->Draw2x2Square(x, y, color);
        }

        err -= dy;
        if (err < 0) {
            y += yChange;
            err += dx;
        }

        x += xChange;
    }
}

void GraphicsModule::Draw2x2Square(int x, int y, int color) {
    this->pixelBuffer[y * CLIENT_SCREEN_WIDTH + x] = color;
    this->pixelBuffer[y * CLIENT_SCREEN_WIDTH + x + 1] = color;
    this->pixelBuffer[(y + 1) * CLIENT_SCREEN_WIDTH + x] = color;
    this->pixelBuffer[(y + 1) * CLIENT_SCREEN_WIDTH + x + 1] = color;
}

void GraphicsModule::UserStateCheck(UserState* userState) {
    if((!userState->ThisPlayerIsAlive()) && this->playerAlive) {
        this->UserStateRedraw(userState);
        this->playerAlive = false;
    }
}

void GraphicsModule::UserStateRedraw(UserState* userState) {
    UINT32* sourcePixels = this->bitmapInfoModule->GetBitmap(BitmapID::MAIN_BACKGROUND)->GetPixels();
    int trueWidth = CLIENT_SCREEN_WIDTH * sizeof(int);

    int currY = 0;
    int row = 0;
    for(; row < GRAPHICS_UI_MAIN_ARENA_INSET_Y; row++) {
        memcpy(this->backBuffer + currY, sourcePixels + currY, trueWidth);
        currY += CLIENT_SCREEN_WIDTH;
    }

    int rightInsetOffset = GRAPHICS_UI_MAIN_ARENA_WIDTH + GRAPHICS_UI_MAIN_ARENA_INSET_X;
    int insetTrueWidth = GRAPHICS_UI_MAIN_ARENA_INSET_X * sizeof(int);
    for(; row < GRAPHICS_UI_MAIN_ARENA_HEIGHT + GRAPHICS_UI_MAIN_ARENA_INSET_Y; row++) {
        memcpy(this->backBuffer + currY, sourcePixels + currY, insetTrueWidth);
        memcpy(this->backBuffer + currY + rightInsetOffset, sourcePixels + currY + rightInsetOffset, insetTrueWidth);
        currY += CLIENT_SCREEN_WIDTH;
    }

    for(; row < CLIENT_SCREEN_HEIGHT; row++) {
        memcpy(this->backBuffer + currY, sourcePixels + currY, trueWidth);
        currY += CLIENT_SCREEN_WIDTH;
    }

    this->DrawNormalBitmap(this->backBuffer, BitmapID::UI_CONTINUE_BUTTON, GRAPHICS_UI_MAIN_BUTTONS_CONTINUE[0].first, GRAPHICS_UI_MAIN_BUTTONS_CONTINUE[0].second);
}
