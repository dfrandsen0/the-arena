#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <fstream>

#include "..\..\config.h"
#include "bitmapInfo.h"
#include "bitmapObj.h"

#include "sprites.h"

BitmapInfo::BitmapInfo() : bitmaps{} {
    
}

BitmapInfo::~BitmapInfo() {
    for(int i = 0; i < GRAPHICS_NUM_BITMAPS; i++) {
        delete bitmaps[i];
    }
}

void BitmapInfo::LoadPPM(BitmapID id) {
    if(this->bitmaps[static_cast<size_t>(id)] != nullptr) {
        return;
    }

    std::string filename = "Resources\\bitmaps\\binaries\\" + Sprites::EncodingToFilename(id);

    std::ifstream file(filename, std::ios::binary);
    std::string magic;

    int width, height, maxval;

    file >> magic >> width >> height >> maxval;
    file.get();

    UINT32* pixels = new UINT32[width * height];

    size_t numBytes = width * height * 3;
    unsigned char* rawData = new unsigned char[numBytes];
    file.read(reinterpret_cast<char*>(rawData), numBytes);

    unsigned char r, g, b;
    for (int i = 0; i < width * height; ++i) {
        r = rawData[i * 3 + 0];
        g = rawData[i * 3 + 1];
        b = rawData[i * 3 + 2];
        pixels[i] = 0xFF000000 | (r << 16) | (g << 8) | b;
    }

    file.close();

    BitmapObj* newBitmap = new BitmapObj(id, width, height, pixels);
    this->bitmaps[static_cast<size_t>(id)] = newBitmap;

    delete[] rawData;
}

BitmapObj* BitmapInfo::GetBitmap(BitmapID ID) {
    return bitmaps[static_cast<size_t>(ID)];
}