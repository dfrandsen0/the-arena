#include "worldUpdate.h"
#include "..\..\config.h"

WorldUpdate::WorldUpdate(int worldX, int worldY, WorldStateID stateId) {
    this->worldX = worldX;
    this->worldY = worldY;
    this->stateId = stateId;
}

WorldUpdate::~WorldUpdate() {

}