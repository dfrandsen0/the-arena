#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include "bitmapObj.h"
#include "..\..\config.h"

BitmapObj::BitmapObj(BitmapID ID, int w, int h, UINT32* pixels) {
    this->ID = ID;
    this->w = w;
    this->h = h;
    this->pixels = pixels;
}

BitmapObj::~BitmapObj() {
    delete[] this->pixels;
}

int BitmapObj::GetWidth() {
    return this->w;
}

int BitmapObj::GetHeight() {
    return this->h;
}

UINT32* BitmapObj::GetPixels() {
    return this->pixels;
}