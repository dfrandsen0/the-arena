#ifndef GRAPHICS_BITMAPINFO_IDTONAME_H_
#define GRAPHICS_BITMAPINFO_IDTONAME_H_

#include <unordered_map>
#include <iostream>
#include "..\..\config.h"

struct BitmapIDHash {
    size_t operator()(BitmapID id) const {
        return static_cast<size_t>(id);
    }
};

struct WorldStateIDHash {
    size_t operator()(WorldStateID id) const {
        return static_cast<size_t>(id);
    }
};

class Sprites {
private:
    Sprites();

    static std::unordered_map<BitmapID, std::string, BitmapIDHash> fileMappings;
    static std::unordered_map<WorldStateID, BitmapID, WorldStateIDHash> worldMappings;
public:
    static std::string EncodingToFilename(BitmapID id);
    static BitmapID WorldStateIDToBitmapID(WorldStateID id);
};

#endif