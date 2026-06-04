#include <iostream>
#include <utility>
#include <bitset>
#include <stack>

#include "ai.h"
#include "..\gameState.h"
#include "..\GameStructure\worldState.h"
#include "..\GameStructure\bucket.h"
#include "..\GameObjects\unit.h"
#include "..\GameObjects\goal.h"
#include "pathFinder.h"
#include "ValueIteration\valueIteration.h"

AIModule::AIModule(GameState* gameState, int numPlayers, std::pair<int, int>* playersToSides) {
    this->gameState = gameState;

    this->criticalCellsSize = numPlayers * MECHANICS_NUM_GOALS;
    this->criticalCells = new std::pair<int, int>[this->criticalCellsSize];

    this->fillWidth = MECHANICS_WORLD_STATE_WIDTH - 2;
    this->fillHeight = MECHANICS_WORLD_STATE_HEIGHT - 2;

    Goal* currGoal;
    int goalX, goalY;
    int index = 0;

    criticalCellsBits.reset();

    for(int i = 0; i < numPlayers; i++) {
        for(int j = 1; j <= MECHANICS_NUM_GOALS; j++) {
            currGoal = this->gameState->GetGoal(playersToSides[i].first, j);
            goalX = currGoal->GetX();
            goalY = currGoal->GetY();

            if(goalX == 0) {
                goalX++;
            } else if(goalY == 0) {
                goalY++;
            } else if(goalX == MECHANICS_WORLD_STATE_WIDTH - 1) {
                goalX--;
            } else if(goalY == MECHANICS_WORLD_STATE_HEIGHT - 1) {
                goalY--;
            }

            this->criticalCells[index] = {goalX, goalY};
            criticalCellsBits.set(this->WorldToBit(goalX, goalY));
            index++;
        }
    }

    this->startBit = this->WorldToBit(this->criticalCells[0].first, this->criticalCells[0].second);

    filledVisited.reset();
    worldBits.reset();
    std::stack<int>().swap(filledStack);

    this->pathfinder = new ValueIteration(this->criticalCells, this->criticalCellsSize);
}

AIModule::~AIModule() {
    delete[] this->criticalCells;
    delete this->pathfinder;
}

PathFinder* AIModule::GetPathFinder() {
    return this->pathfinder;
}

bool AIModule::IsFillable(int worldX, int worldY) {
    if((worldX == 0) ||
       (worldY == 0) ||
       (worldX == MECHANICS_WORLD_STATE_WIDTH - 1) ||
       (worldY == MECHANICS_WORLD_STATE_HEIGHT - 1)) {
        return false;
    }

    WorldState* worldState = this->gameState->GetWorldState();
    if(!(worldState->IsEmpty(worldX, worldY))) {
        return false;
    }

    for(int i = 0; i < this->criticalCellsSize; i++) {
        if((worldX == this->criticalCells[i].first) &&
           (worldY == this->criticalCells[i].second)) {
            return false;
        }
    }

    for(int i = -1; i < 2; i++) {
        if(this->FillableScanBucket(this->gameState->GetBucket(worldX + i), worldX, worldY)) {
            return false;
        }
    }

    //temporarily add possible new
    this->AddWorldBit(worldX, worldY);

    //DFS
    filledVisited.reset();
    std::stack<int>().swap(filledStack);

    filledStack.push(this->startBit);
    filledVisited.set(this->startBit);

    int currState;
    while(!filledStack.empty()) {
        currState = filledStack.top();
        filledStack.pop();

        //left
        if((currState % this->fillWidth != 0) &&
            !filledVisited.test(currState - 1) &&
            !worldBits.test(currState - 1)) {

            filledStack.push(currState - 1);
            filledVisited.set(currState - 1);
        }

        //right
        if(((currState % this->fillWidth) != (this->fillWidth - 1)) &&
             !filledVisited.test(currState + 1) &&
             !worldBits.test(currState + 1)) {

            filledStack.push(currState + 1);
            filledVisited.set(currState + 1);
        }

        //up
        if((currState >= this->fillWidth) &&
            !filledVisited.test(currState - this->fillWidth) &&
            !worldBits.test(currState - this->fillWidth)) {

            filledStack.push(currState - this->fillWidth);
            filledVisited.set(currState - this->fillWidth);
        }

        //down
        if((currState < (this->fillHeight - 1) * this->fillWidth) &&
            !filledVisited.test(currState + this->fillWidth) &&
            !worldBits.test(currState + this->fillWidth)) {

            filledStack.push(currState + this->fillWidth);
            filledVisited.set(currState + this->fillWidth);
        }
    }

    //remove temporary new
    this->RemoveWorldBit(worldX, worldY);

    if((this->criticalCellsBits & this->filledVisited) == this->criticalCellsBits) {
        return true;
    } else {
        return false;
    }
}

bool AIModule::FillableScanBucket(Bucket* bucket, int cellX, int cellY) {
    int unitWorldX, unitWorldY, unitX, unitY;
    this->CellToWorld(cellX, cellY, &unitWorldX, &unitWorldY);

    Unit** currUnits = bucket->GetUnits();
    for(int i = 0; i < bucket->GetSize(); i++) {
        this->UnitToWorld(currUnits[i]->GetX(), currUnits[i]->GetY(), &unitX, &unitY);

        if(this->CheckUnitTouchWorldCell(unitX, unitY, unitWorldX, unitWorldY)) {
            return true;
        }
    }

    return false;
}

void AIModule::UpdateAIWorld(AIUpdateType type, int playerNum, int worldX, int worldY) {
    this->pathfinder->WorldUpdate(type, playerNum, worldX, worldY);

    switch(type) {
        case AIUpdateType::ADD_WALL:
        case AIUpdateType::ADD_TOWER:
            this->AddWorldBit(worldX, worldY);
            break;
        case AIUpdateType::REMOVE_TOWER:
        case AIUpdateType::REMOVE_WALL:
            this->RemoveWorldBit(worldX, worldY);
            break;
        case AIUpdateType::REMOVE_GOAL:
            //handled elsewhere. Needs specific goal.
            break;
    }
}

void AIModule::RemoveGoal(Goal* goal) {
    int cellX = goal->GetX();
    int cellY = goal->GetY();

    int cpX = cellX;
    int cpY = cellY;
    if(cellX == 0) {
        cpX++;
    } else if(cellY == 0) {
        cpY++;
    } else if(cellX == MECHANICS_WORLD_STATE_WIDTH - 1) {
        cpX--;
    } else if(cellY == MECHANICS_WORLD_STATE_HEIGHT - 1) {
        cpY--;
    }

    this->criticalCellsBits.reset(this->WorldToBit(cpX, cpY));

    for(int i = 0; i < this->criticalCellsSize; i++) {
        if(cpX == criticalCells[i].first &&
           cpY == criticalCells[i].second) {

            this->criticalCellsSize--;
            criticalCells[i].first = criticalCells[criticalCellsSize].first;
            criticalCells[i].second = criticalCells[criticalCellsSize].second;
            break;
        }
    }

    this->pathfinder->RemoveGoal(goal);
}

UnitDirection AIModule::GetUnitDirection(Unit* currUnit, bool* withinGoal) {
    *withinGoal = false;

    int worldX, worldY, cellX, cellY;
    this->UnitToWorld(currUnit->GetX(), currUnit->GetY(), &worldX, &worldY);
    this->WorldToCell(worldX, worldY, &cellX, &cellY);

    //check within goals
    if(cellX == 0) {
        return UnitDirection::RIGHT;
    } else if(cellY == 0) {
        return UnitDirection::DOWN;
    } else if(cellX == MECHANICS_WORLD_STATE_WIDTH - 1) {
        return UnitDirection::LEFT;
    } else if(cellY == MECHANICS_WORLD_STATE_HEIGHT - 1) {
        return UnitDirection::UP;
    }

    if(this->CheckGoalCollision(currUnit, cellX, cellY, worldX, worldY)) {
        *withinGoal = true;
        return UnitDirection::UP;
    }

    return this->pathfinder->GetDirection(static_cast<int>(currUnit->GetColor()), worldX, worldY, cellX, cellY);
}

void AIModule::CellToWorld(int cellX, int cellY, int* worldX, int* worldY) {
    *worldX = cellX * GRAPHICS_UI_MAIN_ARENA_CELL_SIZE;
    *worldY = cellY * GRAPHICS_UI_MAIN_ARENA_CELL_SIZE;
}

bool AIModule::CheckUnitTouchWorldCell(int unitX, int unitY, int worldX, int worldY) {
    if((unitX > (worldX - GRAPHICS_BITMAPS_UNIT_OFFSET)) &&
        (unitX <= (worldX + MECHANICS_AI_FILLABLE_ARENA_WITH_OFFSET)) && 
        (unitY > (worldY - GRAPHICS_BITMAPS_UNIT_OFFSET)) &&
        (unitY <= (worldY + MECHANICS_AI_FILLABLE_ARENA_WITH_OFFSET))) {
        return true;
    } else {
        return false;
    }
}

bool AIModule::CheckGoalCollision(Unit* unit, int cellX, int cellY, int worldX, int worldY) {
    if(!this->criticalCellsBits.test(WorldToBit(cellX, cellY))) {
        return false;
    }

    Goal* possibleGoal = this->gameState->CheckGoalOverlap(worldX, worldY);
    if(possibleGoal == nullptr ||
      (static_cast<int>(unit->GetColor()) == possibleGoal->GetPlayerNum())) {
        return false;
    }

    possibleGoal->DecrementHealth();
    return true;
}

void AIModule::AddWorldBit(int worldX, int worldY) {
    worldBits.set(this->WorldToBit(worldX, worldY));
}

void AIModule::RemoveWorldBit(int worldX, int worldY) {
    worldBits.reset(this->WorldToBit(worldX, worldY));
}

int AIModule::WorldToBit(int worldX, int worldY) {
    return (worldY - 1) * this->fillWidth + (worldX - 1);
}

void AIModule::UnitToWorld(int unitX, int unitY, int* worldX, int* worldY) {
    *worldX = unitX / MECHANICS_UNIT_SPACE_AMPLIFIER;
    *worldY = unitY / MECHANICS_UNIT_SPACE_AMPLIFIER;
}

void AIModule::WorldToCell(int worldX, int worldY, int* cellX, int* cellY) {
    *cellX = worldX / GRAPHICS_UI_MAIN_ARENA_CELL_SIZE;
    *cellY = worldY / GRAPHICS_UI_MAIN_ARENA_CELL_SIZE;
}
