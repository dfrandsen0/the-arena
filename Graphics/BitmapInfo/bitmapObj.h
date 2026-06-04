#ifndef GRAPHICS_BITMAPINFO_BITMAP_H_
#define GRAPHICS_BITMAPINFO_BITMAP_H_

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include "..\..\config.h"

class BitmapObj {
private:
    BitmapID ID;
    int w;
    int h;
    UINT32* pixels;
public:
    BitmapObj(BitmapID ID, int w, int h, UINT32* pixels);
    ~BitmapObj();
    int GetWidth();
    int GetHeight();
    UINT32* GetPixels();
};


#endif