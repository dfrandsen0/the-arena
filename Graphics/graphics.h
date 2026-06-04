#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <utility>

#include "Direct2D\direct2D.h"
#include "BitmapInfo\bitmapInfo.h"
#include "..\Mechanics\GameObjects\unit.h"
#include "..\Mechanics\GameObjects\tower.h"
#include "..\Mechanics\mechanics.h"
#include "..\Mechanics\userState.h"
#include "..\config.h"

class GraphicsModule {
public:
private:
    Direct2DModule* direct2DModule = nullptr;
    BitmapInfo* bitmapInfoModule = nullptr;
    HWND hwnd = NULL;

    UINT32* pixelBuffer = nullptr;
    UINT32* backBuffer = nullptr;

    bool playerAlive = true;
public:
    ~GraphicsModule();
    void Reset();

    HWND GetHwnd();

    UINT32* GetPixelBuffer();
    UINT32* GetBackBuffer();

    bool InitGraphics(HINSTANCE hInstance, int nCmdShow);

    void RenderFrame();
    void PushBackBuffer();

    void ImportLoadingMaps();
    void ImportAllMaps();

    void DrawPrettyColors(int offset);
    void DrawUIBackground();
    void DrawLoadingScreen(int loadLevel);

    void DrawUI(UIState state, int pressedIndexFromTop=-1);
    void DrawButtonFlavors(int index, bool pressed, UIState state, int baseX, int baseY);

    void DrawWorldState(WorldStateID* world);
    void DrawMoney(int moneyValue);
    void DrawGoals(MechanicsModule* mechanicsModule);

    void DrawNormalBitmap(UINT32* buffer, BitmapID bitmapID, int x, int y);

    void UpdateGraphics(MechanicsModule* mechanicsModule);

    void DrawUnit(Unit* unit);

    std::pair<int, int> GetBitmapSize(BitmapID id);
    bool WithinButton(int clickX, int clickY, int buttonX, int buttonY, BitmapID id);
private:
    bool CreateWindowModule(HINSTANCE hInstance, int nCmdShow);
    
    void CellToScreen(int cellX, int cellY, int* clientX, int* clientY);
    void WorldToScreen(int worldX, int worldY, int* clientX, int* clientY);
    void ItemCenterScreen(int itemX, int itemY, int* clientX, int* clientY);

    void DrawSquare(Unit* unit);
    void DrawTriangle(Unit* unit);
    void DrawCircle(Unit* unit);

    void DrawTowerLaser(Tower* tower);

    void DrawBitmapUnderCursor(MechanicsModule* mechanicsModule);
    void DrawTowerRadius(int worldX, int worldY, bool fillable);

    void DrawLine(int startX, int startY, int endX, int endY, int color = 0);
    void Draw2x2Square(int x, int y, int color);

    void UserStateCheck(UserState* userState);
    void UserStateRedraw(UserState* userState);
};

#endif
