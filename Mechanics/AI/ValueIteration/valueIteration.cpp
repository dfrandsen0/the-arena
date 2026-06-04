#include <utility>
#include <cstring>
#include <iostream>

#include "valueIteration.h"
#include "valuesObj.h"
#include "..\..\GameObjects\goal.h"
#include "..\..\..\config.h"

ValueIteration::ValueIteration(std::pair<int, int>* criticalCells, int criticalCellsSize) : PathFinder(criticalCells, criticalCellsSize) {
    this->straightDirections = new int[5]{
        -MECHANICS_WORLD_STATE_WIDTH, //up
        1,                            //right
        MECHANICS_WORLD_STATE_WIDTH,  //down
        -1,                           //left
        -MECHANICS_WORLD_STATE_WIDTH //for better computing of diagonals (up again)
    };

    this->diagonalDirections = new int[4]{
        -MECHANICS_WORLD_STATE_WIDTH + 1, //top right
        MECHANICS_WORLD_STATE_WIDTH + 1, //bottom right
        MECHANICS_WORLD_STATE_WIDTH - 1, //bottom left
        -MECHANICS_WORLD_STATE_WIDTH - 1 //top left
    };

    std::queue<int>().swap(this->cellUpdateQueue);
    std::queue<int>().swap(this->helperQueue);
    std::queue<int>().swap(this->originalCells);
    std::stack<int>().swap(this->importanceStack);

    this->visited.reset();
    this->importantCells.reset();
    this->unimportantCells.reset();
}

ValueIteration::~ValueIteration() {
    delete[] this->straightDirections;
    delete[] this->diagonalDirections;

    for(int i = 0; i < this->valueSetsSize; i++) {
        delete valueSets[i];
    }

    delete[] valueSets;
}

void ValueIteration::InitPathFinder(int numPlayers, std::pair<int, int>* playersToSides) {
    this->valueSets = new ValueObj*[numPlayers];
    this->valueSetsSize = numPlayers;

    for(int i = 0; i < numPlayers; i++) {
        this->valueSets[i] = new ValueObj(i);
        this->InitValueSet(i, numPlayers, playersToSides);
    }
}

//Get true direction
UnitDirection ValueIteration::GetDirection(int playerNum, int worldX, int worldY, int cellX, int cellY) {
    ValueObj* valueSet = this->FindValueSet(playerNum);
    int cellIndex = this->CellToIndex(cellX, cellY);

    UnitDirection rawDirection;

    //if in a critical point, go to that goal
    if(valueSet->criticalBits.test(cellIndex)) {
        if(cellX == 1) {
            rawDirection = UnitDirection::LEFT;
        } else if(cellY == 1) {
            rawDirection = UnitDirection::UP;
        } else if(cellX == MECHANICS_WORLD_STATE_WIDTH - 2) {
            rawDirection = UnitDirection::RIGHT;
        } else if(cellY == MECHANICS_WORLD_STATE_HEIGHT - 2) {
            rawDirection = UnitDirection::DOWN;
        }
    } else {
        rawDirection = this->GetRawDirection(valueSet, cellIndex);
    }

    int relativeX = worldX % GRAPHICS_UI_MAIN_ARENA_CELL_SIZE;
    int relativeY = worldY % GRAPHICS_UI_MAIN_ARENA_CELL_SIZE;

    switch(rawDirection) {
        case UnitDirection::UP:
            if((relativeX < GRAPHICS_BITMAPS_UNIT_OFFSET) &&
               (valueSet->worldBits.test(cellIndex + this->diagonalDirections[3]))) {
                return UnitDirection::RIGHT;
            } else if((relativeX >= (GRAPHICS_UI_MAIN_ARENA_CELL_SIZE - GRAPHICS_BITMAPS_UNIT_OFFSET)) &&
                      (valueSet->worldBits.test(cellIndex + this->diagonalDirections[0]))) {
                return UnitDirection::LEFT;
            }
            break;
        case UnitDirection::DOWN:
            if((relativeX < GRAPHICS_BITMAPS_UNIT_OFFSET) &&
               (valueSet->worldBits.test(cellIndex + this->diagonalDirections[2]))) {
                return UnitDirection::RIGHT;
            } else if((relativeX >= (GRAPHICS_UI_MAIN_ARENA_CELL_SIZE - GRAPHICS_BITMAPS_UNIT_OFFSET)) &&
                      (valueSet->worldBits.test(cellIndex + this->diagonalDirections[1]))) {
                return UnitDirection::LEFT;
            }
            break;

        case UnitDirection::LEFT:
            if((relativeY < GRAPHICS_BITMAPS_UNIT_OFFSET) &&
               (valueSet->worldBits.test(cellIndex + this->diagonalDirections[3]))) {
                return UnitDirection::DOWN;
            } else if((relativeY >= (GRAPHICS_UI_MAIN_ARENA_CELL_SIZE - GRAPHICS_BITMAPS_UNIT_OFFSET)) &&
                      (valueSet->worldBits.test(cellIndex + this->diagonalDirections[2]))) {
                return UnitDirection::UP;
            }
            break;
        case UnitDirection::RIGHT:
            if((relativeY < GRAPHICS_BITMAPS_UNIT_OFFSET) &&
               (valueSet->worldBits.test(cellIndex + this->diagonalDirections[0]))) {
                return UnitDirection::DOWN;
            } else if((relativeY >= (GRAPHICS_UI_MAIN_ARENA_CELL_SIZE - GRAPHICS_BITMAPS_UNIT_OFFSET)) &&
                      (valueSet->worldBits.test(cellIndex + this->diagonalDirections[1]))) {
                return UnitDirection::UP;
            }
            break;

        case UnitDirection::UP_RIGHT:
            if((relativeY == GRAPHICS_BITMAPS_UNIT_OFFSET) &&
               (relativeX < GRAPHICS_BITMAPS_UNIT_OFFSET) &&
               (valueSet->worldBits.test(cellIndex + this->diagonalDirections[3]))) {
                return UnitDirection::RIGHT;
            } else if((relativeX == GRAPHICS_UI_MAIN_ARENA_CELL_SIZE - GRAPHICS_BITMAPS_UNIT_OFFSET - 1) &&
                      (relativeY >= (GRAPHICS_UI_MAIN_ARENA_CELL_SIZE - GRAPHICS_BITMAPS_UNIT_OFFSET)) &&
                      (valueSet->worldBits.test(cellIndex + this->diagonalDirections[1]))) {
                return UnitDirection::UP;
            }
            break;
        
        case UnitDirection::DOWN_LEFT:
            if((relativeX == GRAPHICS_BITMAPS_UNIT_OFFSET) &&
               (relativeY < GRAPHICS_BITMAPS_UNIT_OFFSET) &&
               (valueSet->worldBits.test(cellIndex + this->diagonalDirections[3]))) {
                return UnitDirection::DOWN;
            } else if((relativeY == GRAPHICS_UI_MAIN_ARENA_CELL_SIZE - GRAPHICS_BITMAPS_UNIT_OFFSET - 1) &&
                      (relativeX >= GRAPHICS_UI_MAIN_ARENA_CELL_SIZE - GRAPHICS_BITMAPS_UNIT_OFFSET) &&
                      (valueSet->worldBits.test(cellIndex + this->diagonalDirections[1]))) {
                return UnitDirection::LEFT;
            }
            break;

        case UnitDirection::UP_LEFT:
            if((relativeX == GRAPHICS_BITMAPS_UNIT_OFFSET) &&
               (relativeY >= GRAPHICS_UI_MAIN_ARENA_CELL_SIZE - GRAPHICS_BITMAPS_UNIT_OFFSET) &&
               (valueSet->worldBits.test(cellIndex + this->diagonalDirections[2]))) {
                return UnitDirection::UP;
            } else if((relativeY == GRAPHICS_BITMAPS_UNIT_OFFSET) &&
                      (relativeX >= GRAPHICS_UI_MAIN_ARENA_CELL_SIZE - GRAPHICS_BITMAPS_UNIT_OFFSET) &&
                      (valueSet->worldBits.test(cellIndex + this->diagonalDirections[0]))) {
                return UnitDirection::LEFT;
            }
            break;

        case UnitDirection::DOWN_RIGHT:
            if((relativeY == GRAPHICS_UI_MAIN_ARENA_CELL_SIZE - GRAPHICS_BITMAPS_UNIT_OFFSET - 1) &&
               (relativeX < GRAPHICS_BITMAPS_UNIT_OFFSET) &&
               (valueSet->worldBits.test(cellIndex + this->diagonalDirections[2]))) {
                return UnitDirection::RIGHT;
            } else if((relativeX == GRAPHICS_UI_MAIN_ARENA_CELL_SIZE - GRAPHICS_BITMAPS_UNIT_OFFSET - 1) &&
                      (relativeY < GRAPHICS_BITMAPS_UNIT_OFFSET) &&
                      (valueSet->worldBits.test(cellIndex + this->diagonalDirections[0]))) {
                return UnitDirection::DOWN;
            }
            break;
    }

    return rawDirection;
}

void ValueIteration::WorldUpdate(AIUpdateType type, int playerNum, int cellX, int cellY) {
    int index = this->CellToIndex(cellX, cellY);

    switch(type) {
        case AIUpdateType::ADD_WALL:
        case AIUpdateType::ADD_TOWER:
            this->SetWorldBit(index);
            break;
        case AIUpdateType::REMOVE_WALL:
        case AIUpdateType::REMOVE_TOWER:
            this->ResetWorldBit(index);
            this->cellUpdateQueue.push(index);
            break;
        case AIUpdateType::REMOVE_GOAL:
            this->SetWorldBit(index);

            //changes index from goal to respective CP
            if(cellX == 0) {
                index += 1;
            } else if(cellY == 0) {
                index += MECHANICS_WORLD_STATE_WIDTH;
            } else if(cellX == MECHANICS_WORLD_STATE_WIDTH - 1) {
                index -= 1;
            } else if(cellY == MECHANICS_WORLD_STATE_HEIGHT - 1) {
                index -= MECHANICS_WORLD_STATE_HEIGHT;
            }

            //some redundancy, since all 4 critical bits are being unset, but
            //  it doesn't hurt, and checking if it's for the right player
            //  would be more expensive.
            for(int i = 0; i < valueSetsSize; i++) {
                this->valueSets[i]->criticalBits.reset(index);
            }

            this->cellUpdateQueue.push(index);
            this->ChangeGoalReward(playerNum, index);

            break;
    }

    for(int i = 0; i < 4; i++) {
        this->cellUpdateQueue.push(index + this->straightDirections[i]);
    }

    if(type == AIUpdateType::ADD_TOWER ||
       type == AIUpdateType::REMOVE_TOWER) {
        this->ChangeTowerReward(type, playerNum, cellX, cellY);
    }
}

void ValueIteration::UpdatePathFinding() {
    if(this->cellUpdateQueue.size() == 0) {
        return;
    }

    std::queue<int>().swap(this->helperQueue);
    std::queue<int>().swap(this->originalCells);
    this->importantCells.reset();
    this->unimportantCells.reset();

    ValueObj* valueSet = this->valueSets[0];

    //remove from queue any that are walls
    int currIndex;
    while(!cellUpdateQueue.empty()) {
        currIndex = cellUpdateQueue.front();
        cellUpdateQueue.pop();

        if(!valueSet->worldBits.test(currIndex)) {
            helperQueue.push(currIndex);
        }
    }
    
    //remove any from queue that fail DFS CP test
    bool currImportant;
    while(!helperQueue.empty()) {
        currIndex = helperQueue.front();
        helperQueue.pop();

        if(this->importantCells.test(currIndex)) {
            originalCells.push(currIndex);
            continue;
        }

        if(this->unimportantCells.test(currIndex)) {
            continue;
        }

        currImportant = this->CheckImportance(currIndex);
        if(currImportant) {
            this->importantCells |= this->visited;
            originalCells.push(currIndex);
        } else {
            this->unimportantCells |= this->visited;
        }
    }

    if(originalCells.empty()) {
        return;
    }

    //update cells and percolate across affected cells
    int queueSize;
    this->visited.reset();

    for(int v = 0; v < this->valueSetsSize; v++) {
        valueSet = this->valueSets[v];
        cellUpdateQueue = originalCells;

        while(!cellUpdateQueue.empty()) {
            queueSize = (int)cellUpdateQueue.size();

            for(int i = 0; i < queueSize; i++) {
                currIndex = cellUpdateQueue.front();
                cellUpdateQueue.pop();

                if(this->UpdateCell(valueSet, currIndex)) {
                    this->AddAffectedCells(valueSet, currIndex);
                }
            }

            cellUpdateQueue.swap(helperQueue);
            this->visited.reset();
        }
    }

    std::queue<int>().swap(this->cellUpdateQueue);
}

void ValueIteration::RemoveGoal(Goal* goal) {
    this->WorldUpdate(AIUpdateType::REMOVE_GOAL, goal->GetPlayerNum(), goal->GetX(), goal->GetY());
}

void ValueIteration::InitValueSet(int index, int numPlayers, std::pair<int, int>* playersToSides) {
    ValueObj* valueSet = this->valueSets[index];
    valueSet->playerNum = playersToSides[index].first;
    
    //critical point x/y
    int cpX, cpY, worldIndex;

    for(int i = 0; i < numPlayers; i++) {
        if(i == index) {
            continue;
        }

        for(int j = 0; j < MECHANICS_NUM_GOALS; j++) {
            cpX = this->criticalCells[(i * MECHANICS_NUM_GOALS) + j].first;
            cpY = this->criticalCells[(i * MECHANICS_NUM_GOALS) + j].second;
            worldIndex = this->CellToIndex(cpX, cpY);
            valueSet->rewards[worldIndex] = MECHANICS_AI_VALUE_ITERATION_CP_VALUE;
            valueSet->criticalBits.set(worldIndex);
        }
    }

    int lastRowOffset = MECHANICS_WORLD_STATE_HEIGHT * (MECHANICS_WORLD_STATE_WIDTH - 1);
    int yValue = 0;
    int offset = MECHANICS_WORLD_STATE_WIDTH - 1;

    for(int i = 0; i < MECHANICS_WORLD_STATE_WIDTH; i++) {
        valueSet->worldBits.set(i);
        valueSet->worldBits.set(i + lastRowOffset);
        valueSet->worldBits.set(yValue);
        valueSet->worldBits.set(yValue + offset);
        yValue += MECHANICS_WORLD_STATE_WIDTH;
    }

    bool needsUpdate = true;
    while(needsUpdate) {
        needsUpdate = false;
        for(int y = 1; y < (MECHANICS_WORLD_STATE_HEIGHT - 1); y++) {
            for(int x = 1; x < (MECHANICS_WORLD_STATE_WIDTH - 1); x++) {
                if(this->UpdateCell(valueSet, this->CellToIndex(x, y))) {
                    needsUpdate = true;
                }
            }
        }
    }
}

ValueObj* ValueIteration::FindValueSet(int playerNum) {
    for(int i = 0; i < this->valueSetsSize; i++) {
        if(this->valueSets[i]->playerNum == playerNum) {
            return this->valueSets[i];
        }
    }

    std::cout << "Couldn't find value set! Problem!" << std::endl;
    return nullptr;
}

void ValueIteration::SetWorldBit(int index) {
    for(int i = 0; i < this->valueSetsSize; i++) {
        this->valueSets[i]->worldBits.set(index);
    }
}

void ValueIteration::ResetWorldBit(int index) {
    for(int i = 0; i < this->valueSetsSize; i++) {
        this->valueSets[i]->worldBits.reset(index);
    }
}

void ValueIteration::ChangeGoalReward(int playerNum, int index) {
    for(int i = 0; i < this->valueSetsSize; i++) {
        if(this->valueSets[i]->playerNum != playerNum) {
            this->valueSets[i]->rewards[index] -= MECHANICS_AI_VALUE_ITERATION_CP_VALUE;
        }
    }
}

void ValueIteration::ChangeTowerReward(AIUpdateType type, int playerNum, int cellX, int cellY) {
    int reward;
    if(type == AIUpdateType::REMOVE_TOWER) {
        reward = MECHANICS_AI_VALUE_ITERATION_TOWER_COST;
    } else {
        reward = -MECHANICS_AI_VALUE_ITERATION_TOWER_COST;
    }

    cellX -= MECHANICS_TOWER_RADIUS;
    cellY -= MECHANICS_TOWER_RADIUS;
    int endCellX = cellX + MECHANICS_TWICE_TOWER_RADIUS;
    int endCellY = cellY + MECHANICS_TWICE_TOWER_RADIUS;

    int index;

    for(int y = cellY; y <= endCellY; y++) {
        for(int x = cellX; x <= endCellX; x++) {
            if(x < 0 ||
               y < 0 ||
               x >= MECHANICS_WORLD_STATE_WIDTH ||
               y >= MECHANICS_WORLD_STATE_HEIGHT ||
              (y == cellY && x == cellX) ||
              (y == endCellY && x == endCellX) ||
              (y == endCellY && x == cellX) ||
              (y == cellY && x == endCellX)) {
                continue;
            }

            index = this->CellToIndex(x, y);

            for(int i = 0; i < this->valueSetsSize; i++) {
                if(this->valueSets[i]->playerNum != playerNum) {
                    this->valueSets[i]->rewards[index] += reward;
                }
            }
        }
    }
}

bool ValueIteration::CheckImportance(int index) {
    ValueObj* valueSet = this->valueSets[0];

    std::stack<int>().swap(this->importanceStack);
    this->visited.reset();

    importanceStack.push(index);
    visited.set(index);

    int currIndex, nextIndex;
    while(!importanceStack.empty()) {
        currIndex = importanceStack.top();
        importanceStack.pop();

        if(valueSet->criticalBits.test(currIndex)) {
            return true;
        }

        for(int i = 0; i < 4; i++) {
            nextIndex = currIndex + this->straightDirections[i];
            if(!visited.test(nextIndex) &&
               !valueSet->worldBits.test(nextIndex)) {

                importanceStack.push(nextIndex);
                visited.set(nextIndex);
            }
        }
    }

    return false;
}

bool ValueIteration::UpdateCell(ValueObj* valueSet, int index) {
    if(valueSet->worldBits.test(index)) {
        return false;
    }

    if(valueSet->criticalBits.test(index)) {
        if(valueSet->values[index] != valueSet->rewards[index]) {
            valueSet->values[index] = valueSet->rewards[index];
            return true;
        }
        return false;
    }

    int maxVal = this->CalcCellValue(valueSet, index);

    if(valueSet->values[index] != maxVal) {
        valueSet->values[index] = maxVal;
        return true;
    }
    return false;
}

void ValueIteration::AddAffectedCells(ValueObj* valueSet, int index) {
    int nextIndex;
    for(int i = 0; i < 4; i++) {
        nextIndex = index + this->straightDirections[i];
        if(!valueSet->worldBits[nextIndex] &&
           !this->visited.test(nextIndex)) {

            this->helperQueue.push(nextIndex);
            this->visited.set(nextIndex);
        }
    }

    for(int i = 0; i < 4; i++) {
        nextIndex = index + this->diagonalDirections[i];
        if(!this->visited.test(nextIndex) &&
           !valueSet->worldBits[nextIndex] &&
           !valueSet->worldBits[index + this->straightDirections[i]] &&
           !valueSet->worldBits[index + this->straightDirections[i + 1]]) {

            this->helperQueue.push(nextIndex);
            this->visited.set(nextIndex);
        }
    }
}

UnitDirection ValueIteration::GetRawDirection(ValueObj* valueSet, int index) {
    int maxVal = MECHANICS_AI_VALUE_ITERATION_MIN_VALUE;
    UnitDirection direction;
    int tempVal;

    for(int i = 0; i < 4; i++) {
        if(!valueSet->worldBits.test(index + this->straightDirections[i])) {
            tempVal = valueSet->values[index + this->straightDirections[i]];
            if(tempVal > maxVal) {
                maxVal = tempVal;
                direction = static_cast<UnitDirection>(i);
            }
        }
    }

    for(int i = 0; i < 4; i++) {
        if(!valueSet->worldBits.test(index + this->diagonalDirections[i]) &&
           !valueSet->worldBits.test(index + this->straightDirections[i]) &&
           !valueSet->worldBits.test(index + this->straightDirections[i + 1])) {

            tempVal = valueSet->values[index + this->diagonalDirections[i]];
            if(tempVal > maxVal) {
                maxVal = tempVal;
                direction = static_cast<UnitDirection>(i + 4);
            }
        }
    }

    return direction;
}

int ValueIteration::CalcCellValue(ValueObj* valueSet, int index) {
    int maxVal = MECHANICS_AI_VALUE_ITERATION_MIN_VALUE;
    int tempVal;

    for(int i = 0; i < 4; i++) {
        if(!valueSet->worldBits.test(index + this->straightDirections[i])) {
            tempVal = valueSet->values[index + this->straightDirections[i]] - MECHANICS_AI_VALUE_ITERATION_ADJACENT_COST + valueSet->rewards[index];
            if(tempVal > maxVal) {
                maxVal = tempVal;
            }
        }
    }

    for(int i = 0; i < 4; i++) {
        if(!(valueSet->worldBits.test(index + this->diagonalDirections[i])) &&
           !(valueSet->worldBits.test(index + this->straightDirections[i])) &&
           !(valueSet->worldBits.test(index + this->straightDirections[i + 1]))) {
            tempVal = valueSet->values[index + this->diagonalDirections[i]] - MECHANICS_AI_VALUE_ITERATION_DIAGONAL_COST + valueSet->rewards[index];
            if(tempVal > maxVal) {
                maxVal = tempVal;
            }
        }
    }

    return maxVal;
}

int ValueIteration::CellToIndex(int cellX, int cellY) {
    return (cellY * MECHANICS_WORLD_STATE_WIDTH) + cellX;
}
