#ifndef GRPAHICS_DIRECT2D_H_
#define GRAPHICS_DIRECT2D_H_

#include <d2d1.h>

class Direct2DModule {
public:
    Direct2DModule(HWND hwnd);
    ~Direct2DModule();
    void InitD2D(HWND hwnd);
    void CleanupD2D();

    void RenderFrame(UINT32* pixels);
private:
    ID2D1Factory* factory = nullptr;
    ID2D1HwndRenderTarget* renderTarget = nullptr;
    ID2D1Bitmap* bitmap = nullptr;
};

#endif