#ifndef GRAPHICS_BITMAPINFO_H_
#define GRAPHICS_BITMAPINFO_H_

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>

#include "..\..\config.h"
#include "bitmapObj.h"

class BitmapInfo {
public:
private:
    BitmapObj* bitmaps[GRAPHICS_NUM_BITMAPS];

public:
    BitmapInfo();
    ~BitmapInfo();
    void LoadPPM(BitmapID id);
    BitmapObj* GetBitmap(BitmapID ID);
private:
};

#endif