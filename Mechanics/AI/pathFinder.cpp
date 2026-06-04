#include <utility>

#include "pathFinder.h"

PathFinder::PathFinder(std::pair<int, int>* criticalCells, int criticalCellsSize) {
    this->criticalCells = criticalCells;
    this->criticalCellsSize = criticalCellsSize;
}

PathFinder::~PathFinder() {
    
}