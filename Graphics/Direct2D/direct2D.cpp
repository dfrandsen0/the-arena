#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <d2d1.h>

#include "../../config.h"
#include "direct2D.h"

Direct2DModule::Direct2DModule(HWND hwnd) {
    this->InitD2D(hwnd);
}

Direct2DModule::~Direct2DModule() {
    this->CleanupD2D();
}

void Direct2DModule::InitD2D(HWND hwnd) {
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);

    RECT rc;
    GetClientRect(hwnd, &rc);

    D2D1_RENDER_TARGET_PROPERTIES props = 
        D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
                                     D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
                                                       D2D1_ALPHA_MODE_IGNORE));

    D2D1_HWND_RENDER_TARGET_PROPERTIES hwndProps =
        D2D1::HwndRenderTargetProperties(hwnd,
                                         D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top),
                                         D2D1_PRESENT_OPTIONS_NONE); // VSync enabled

    factory->CreateHwndRenderTarget(&props, &hwndProps, &renderTarget);

    // Create initial bitmap
    D2D1_BITMAP_PROPERTIES bmpProps =
        D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
                                                 D2D1_ALPHA_MODE_IGNORE));
    renderTarget->CreateBitmap(D2D1::SizeU(CLIENT_SCREEN_WIDTH, CLIENT_SCREEN_HEIGHT), bmpProps, &bitmap);
}

void Direct2DModule::CleanupD2D() {
    if (bitmap) bitmap->Release();
    if (renderTarget) renderTarget->Release();
    if (factory) factory->Release();
}

void Direct2DModule::RenderFrame(UINT32* pixels) {
    D2D1_RECT_U rect = {0, 0, (UINT32)CLIENT_SCREEN_WIDTH, (UINT32)CLIENT_SCREEN_HEIGHT};
    this->bitmap->CopyFromMemory(&rect, pixels, CLIENT_SCREEN_WIDTH * 4);

    this->renderTarget->BeginDraw();
    this->renderTarget->Clear(D2D1::ColorF(0, 0, 0));
    this->renderTarget->DrawBitmap(this->bitmap);
    this->renderTarget->EndDraw(); // BLOCKS for VSync
}