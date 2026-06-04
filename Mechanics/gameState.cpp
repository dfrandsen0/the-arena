#include <utility>
#include <vector>
#include <iostream>
#include <cmath>

#include "gameState.h"
#include "GameObjects\player.h"
#include "GameObjects\unit.h"
#include "GameObjects\goal.h"
#include "GameObjects\tower.h"
#include "GameObjects\wall.h"
#include "GameStructure\worldState.h"
#include "GameStructure\bucket.h"
#include "..\Listener\mainInput.h"
#include "AI\ai.h"
#include "..\config.h"

GameState::GameState(int numPlayers, std::pair<int, int>* playersToSides) {
    this->worldState = new WorldState(numPlayers, playersToSides);
    this->InitPlayers(numPlayers, playersToSides);
    
    this->units = new Unit*[MECHANICS_GAME_STATE_MAX_UNITS];
    this->unitsSize = MECHANICS_GAME_STATE_MAX_UNITS;
    for(int i = 0; i < this->unitsSize; i++) {
        this->units[i] = nullptr;
    }

    this->buckets = new Bucket*[MECHANICS_WORLD_STATE_WIDTH];
    this->bucketsSize = MECHANICS_WORLD_STATE_WIDTH;
    for(int i = 0; i < this->bucketsSize; i++) {
        this->buckets[i] = new Bucket();
    }
    
    this->InitGoals(numPlayers, playersToSides);

    this->InitTowers(numPlayers);

    this->InitWalls();

    this->aiModule = new AIModule(this, numPlayers, playersToSides);
}

GameState::~GameState() {
    delete this->worldState;

    for(int i = 0; i < playersSize; i++) {
        delete players[i];
    }
    delete[] this->players;

    for(int i = 0; i < goalsSize; i++) {
        delete goals[i];
    }
    delete[] this->goals;

    for(int i = 0; i < unitsSize; i++) {
        delete this->units[i];
    }
    delete[] this->units;

    for(int i = 0; i < bucketsSize; i++) {
        delete this->buckets[i];
    }
    delete[] this->buckets;

    for(int i = 0; i < towersSize; i++) {
        delete this->towers[i];
    }
    delete[] this->towers;

    for(int i = 0; i < wallsSize; i++) {
        delete this->walls[i];
    }
    delete[] this->walls;

    delete this->aiModule;
}

void GameState::ClearBuckets() {
    for(int i = 0; i < this->bucketsSize; i++) {
        this->buckets[i]->Clear();
    }
}

Bucket* GameState::GetBucket(int index) {
    return this->buckets[index];
}

WorldState* GameState::GetWorldState() {
    return this->worldState;
}

AIModule* GameState::GetAIModule() {
    return this->aiModule;
}

bool GameState::AtMaxTowers(int player) {
    for(int i = 0; i < this->playersSize; i++) {
        if(players[i]->GetPlayerNum() == player) {
            if(players[i]->GetNumTowers() == MECHANICS_MAX_TOWERS_PER_PLAYER) {
                return true;
            } else {
                return false;
            }
        }
    }

    std::cout << "Couldn't find player when checking max towers" << std::endl;
    return false;
}

void GameState::FindRelativeXY(int clickX, int clickY, int* worldX, int* worldY) {
    *worldX = (clickX - GRAPHICS_UI_MAIN_ARENA_INSET_X) / GRAPHICS_UI_MAIN_ARENA_CELL_SIZE;
    *worldY = (clickY - GRAPHICS_UI_MAIN_ARENA_INSET_Y) / GRAPHICS_UI_MAIN_ARENA_CELL_SIZE;
}

void GameState::GoalToUnitStart(int goalX, int goalY, int* unitX, int* unitY) {
    *unitX = (goalX * GRAPHICS_UI_MAIN_ARENA_CELL_SIZE + (GRAPHICS_UI_MAIN_ARENA_CELL_SIZE / 2)) * MECHANICS_UNIT_SPACE_AMPLIFIER;
    *unitY = (goalY * GRAPHICS_UI_MAIN_ARENA_CELL_SIZE + (GRAPHICS_UI_MAIN_ARENA_CELL_SIZE / 2)) * MECHANICS_UNIT_SPACE_AMPLIFIER;
}

WorldStateID GameState::GetWorldItem(int worldX, int worldY) {
    return this->worldState->GetItem(worldX, worldY);
}

Tower* GameState::FindTowerByXY(int worldX, int worldY) {
    for(int i = 0; i < this->towersSize; i++) {
        if(towers[i] == nullptr) {
            continue;
        }

        if((towers[i]->GetX() == worldX) && (towers[i]->GetY() == worldY)) {
            return towers[i];
        }
    }
    
    return nullptr;
}

Tower** GameState::GetAllTowers() {
    return this->towers;
}

bool GameState::WithinSelectedRadius(Tower* tower, int worldX, int worldY) {
    int towerX = tower->GetX();
    int towerY = tower->GetY();
    int diffX = std::abs(towerX - worldX);
    int diffY = std::abs(towerY - worldY);

    if(diffX > MECHANICS_TOWER_RADIUS || diffX > MECHANICS_TOWER_RADIUS) {
        return false;
    }

    if(diffX == MECHANICS_TOWER_RADIUS && diffY == MECHANICS_TOWER_RADIUS) {
        return false;
    }

    return true;
}

void GameState::IncrementPlayerMoney() {
    for(int i = 0; i < this->playersSize; i++) {
        this->players[i]->IncrementMoney();
    }
}

int GameState::GetPlayerMoney(int playerNum) {
    for(int i = 0; i < this->playersSize; i++) {
        return this->players[i]->GetMoney();
    }

    return 0;
}

int GameState::GetMoneyDifference(int playerNum) {
    Player* thisPlayer = nullptr;
    for(int i = 0; i < playersSize; i++) {
        if(players[i]->GetPlayerNum() == playerNum) {
            thisPlayer = players[i];
            break;
        }
    }

    if(thisPlayer == nullptr) {
        std::cout << "player " << playerNum << " not found in GetPlayerMoney." << std::endl;
        return 0;
    }

    if((thisPlayer->GetLastMoney() / 1000) != (thisPlayer->GetMoney() / 1000)) {
        return thisPlayer->GetMoney();
    }

    return 0;
}

void GameState::UpdateLastMoneys() {
    for(int i = 0; i < this->playersSize; i++) {
        this->players[i]->UpdateLastMoney();
    }
}

Goal* GameState::GetGoal(int playerNum, int spawnNum) {
    for(int i = 0; i < this->goalsSize; i++) {
        if(goals[i] == nullptr) {
            continue;
        }

        if((goals[i]->GetPlayerNum() == playerNum) && (goals[i]->GetSpawnNum() == spawnNum)) {
            return goals[i];
        }
    }

    return nullptr;
}

Player* GameState::GetPlayer(int playerNum) {
    for(int i = 0; i < this->playersSize; i++) {
        if(players[i]->GetPlayerNum() == playerNum) {
            return players[i];
        }
    }

    return nullptr;
}

int GameState::GetLastUnit() {
    return this->lastUnit;
}

Unit* GameState::GetUnit(int index) {
    return this->units[index];
}

Tower* GameState::GetTower(int id) {
    return this->towers[id];
}

void GameState::HandleInput(MainInput* input, int playerNum) {
    if(this->IsPlayerOut(playerNum)) {
        return;
    }

    switch(input->type) {
        case MainInputID::CREATE_SQUARE:
            this->CreateUnit(playerNum, input->xParam, UnitType::SQUARE);
            break;
        case MainInputID::CREATE_TRIANGLE:
            this->CreateUnit(playerNum, input->xParam, UnitType::TRIANGLE);
            break;
        case MainInputID::CREATE_CIRCLE:
            this->CreateUnit(playerNum, input->xParam, UnitType::CIRCLE);
            break;
        case MainInputID::BUILD_WALL:
            this->CreateWall(playerNum, input->xParam, input->yParam);
            break;
        case MainInputID::BUILD_TOWER:
            this->CreateTower(playerNum, input->xParam, input->yParam);
            break;
        case MainInputID::REMOVE_TOWER:
            this->RemoveTower(playerNum, input->xParam);
            break;
        case MainInputID::TOWER_TARGETS:
            this->ChangeTowerTarget(input->xParam, input->yParam, input->idParam);
            break;
        default:
            break;
    }
}

void GameState::UpdateTowers() {
    for(int i = 0; i < this->towersSize; i++) {
        if(this->towers[i] != nullptr) {
            this->TowerAction(towers[i]);
        }
    }

    for(int i = 0; i < this->towersSize; i++) {
        if(this->towers[i] != nullptr) {
            this->UpdateTowerHealth(towers[i]);
        }
    }
}

void GameState::UpdateWalls() {
    for(int i = 0; i < this->wallsSize; i++) {
        if(this->walls[i] == nullptr) {
            continue;
        }

        this->walls[i]->ShiftAttacks();
        if(this->walls[i]->GetHealth() <= 0) {
            this->RemoveWall(i);
        }
    }
}

void GameState::UpdateUnits() {
    UnitDirection direction;
    Unit* currUnit;
    bool withinGoal;

    this->ClearBuckets();

    for(int i = 0; i < this->lastUnit; i++) {
        currUnit = this->units[i];

        if(currUnit->GetHealth() <= 0) {
            this->RemoveUnit(i);
            i--;
            continue;
        }

        currUnit->DecrementDeathDelay();
        if(currUnit->GetDeathDelay() == 0) {
            this->RemoveUnit(i);
            i--;
            continue;
        }

        //Putting goal checks here rather than post movement delays goal
        //  collisions by a frame, which means the user will at least see
        //  the unit touch the goal for a frame before it disappears.
        direction = this->aiModule->GetUnitDirection(currUnit, &withinGoal);
        if(withinGoal) {
            this->RemoveUnit(i);
            i--;
            continue;
        }

        currUnit->SetTrajectory(direction);

        switch(direction) {
            case UnitDirection::UP:
                currUnit->SetY(currUnit->GetY() - MECHANICS_UNIT_MOVE_ADJACENT);
                break;
            case UnitDirection::DOWN:
                currUnit->SetY(currUnit->GetY() + MECHANICS_UNIT_MOVE_ADJACENT);
                break;
            case UnitDirection::LEFT:
                currUnit->SetX(currUnit->GetX() - MECHANICS_UNIT_MOVE_ADJACENT);
                break;
            case UnitDirection::RIGHT:
                currUnit->SetX(currUnit->GetX() + MECHANICS_UNIT_MOVE_ADJACENT);
                break;
            case UnitDirection::UP_RIGHT:
                currUnit->SetX(currUnit->GetX() + MECHANICS_UNIT_MOVE_DIAGONAL);
                currUnit->SetY(currUnit->GetY() - MECHANICS_UNIT_MOVE_DIAGONAL);
                break;
            case UnitDirection::UP_LEFT:
                currUnit->SetX(currUnit->GetX() - MECHANICS_UNIT_MOVE_DIAGONAL);
                currUnit->SetY(currUnit->GetY() - MECHANICS_UNIT_MOVE_DIAGONAL);
                break;
            case UnitDirection::DOWN_RIGHT:
                currUnit->SetX(currUnit->GetX() + MECHANICS_UNIT_MOVE_DIAGONAL);
                currUnit->SetY(currUnit->GetY() + MECHANICS_UNIT_MOVE_DIAGONAL);
                break;
            case UnitDirection::DOWN_LEFT:
                currUnit->SetX(currUnit->GetX() - MECHANICS_UNIT_MOVE_DIAGONAL);
                currUnit->SetY(currUnit->GetY() + MECHANICS_UNIT_MOVE_DIAGONAL);
                break;
        }

        this->PutUnitInBuckets(currUnit);
    }
}

// Returns true if any goal is killed
bool GameState::UpdateGoals(int thisPlayer) {
    bool killedFlag = false;
    for(int i = 0; i < goalsSize; i++) {
        if(this->goals[i] == nullptr) {
            continue;
        }

        if(this->goals[i]->GetHealth() <= 0) {
            killedFlag = true;

            this->aiModule->RemoveGoal(this->goals[i]);
            this->worldState->PutItem(this->goals[i]->GetX(), this->goals[i]->GetY(), WorldStateID::DEAD_GOAL);

            delete this->goals[i];
            this->goals[i] = nullptr;
        }
    }
    return killedFlag;
}

void GameState::CalculateCollisions() {
    Unit** currBucketUnits;
    Unit** nextBucketUnits;

    Unit* unit1;
    Unit* unit2;

    int currBucketSize, nextBucketSize;

    for(int i = 0; i < this->bucketsSize - 1; i++) {
        currBucketUnits = this->buckets[i]->GetUnits();
        currBucketSize = this->buckets[i]->GetSize();

        if(currBucketSize == 0) {
            continue;
        }

        for(int j = 0; j < currBucketSize; j++) {
            unit1 = currBucketUnits[j];

            for(int k = j + 1; k < currBucketSize; k++) {
                unit2 = currBucketUnits[k];
                if(CheckCollide(unit1, unit2)) {
                    this->DamageUnits(unit1, unit2);

                    if(unit1->GetHealth() <= 0) {
                        break;
                    }
                }
            }

            if(unit1->GetHealth() <= 0) {
                continue;
            }

            nextBucketUnits = this->buckets[i + 1]->GetUnits();
            nextBucketSize = this->buckets[i + 1]->GetSize();

            for(int k = 0; k < nextBucketSize; k++) {
                unit2 = nextBucketUnits[k];
                if(CheckCollide(unit1, unit2)) {
                    this->DamageUnits(unit1, unit2);

                    if(unit1->GetHealth() <= 0) {
                        break;
                    }
                }
            }
        }
    }    
}

Goal* GameState::CheckGoalOverlap(int worldX, int worldY) {
    int goalUnitX, goalUnitY;
    
    for(int i = 0; i < goalsSize; i++) {
        if(this->goals[i] == nullptr) {
            continue;
        }

        this->aiModule->CellToWorld(this->goals[i]->GetX(), this->goals[i]->GetY(), &goalUnitX, &goalUnitY);

        if(this->aiModule->CheckUnitTouchWorldCell(worldX, worldY, goalUnitX, goalUnitY)) {
            return this->goals[i];
        }
    }

    return nullptr;
}

bool GameState::IsPlayerOut(int playerNum) {
    for(int i = 0; i < this->goalsSize; i++) {
        if(goals[i] != nullptr && goals[i]->GetPlayerNum() == playerNum) {
            return false;
        }
    }

    return true;
}

void GameState::InitPlayers(int numPlayers, std::pair<int, int>* playersToSides) {
    this->players = new Player*[numPlayers];
    this->playersSize = numPlayers;

    for(int i = 0; i < numPlayers; i++) {
        this->players[i] = new Player(playersToSides[i].first, playersToSides[i].second);
    }
}

void GameState::InitGoals(int numPlayers, std::pair<int, int>* playersToSides) {
    this->goals = new Goal*[numPlayers * MECHANICS_NUM_GOALS];
    this->goalsSize = numPlayers * MECHANICS_NUM_GOALS;
    std::vector<std::pair<int, int>> thisGoalXYS;

    for(int i = 0; i < numPlayers; i++) {
        thisGoalXYS = MECHANICS_WORLD_STATE_GOALS_XYS[playersToSides[i].second];
        for(int j = 0; j < MECHANICS_NUM_GOALS; j++) {
            this->goals[(i * MECHANICS_NUM_GOALS) + j] = new Goal(playersToSides[i].first, thisGoalXYS[j].first, thisGoalXYS[j].second, j + 1);
        }
    }
}

void GameState::InitTowers(int numPlayers) {
    //for ease sake, we're placing enough space for all players (even in 2 player games)
    //  and then placing into location by player num. Now we don't have to check who
    //  else might be playing when placing a tower
    this->towers = new Tower*[MECHANICS_MAX_NUM_PLAYERS * MECHANICS_MAX_TOWERS_PER_PLAYER];
    this->towersSize = MECHANICS_MAX_NUM_PLAYERS * MECHANICS_MAX_TOWERS_PER_PLAYER;
    for(int i = 0; i < this->towersSize; i++) {
        this->towers[i] = nullptr;
    }
}

void GameState::InitWalls() {
    this->wallsSize = MECHANICS_WORLD_STATE_WIDTH * MECHANICS_WORLD_STATE_HEIGHT;
    this->walls = new Wall*[wallsSize];

    //wallssize is a little extra big; there will be a border of nullptrs where
    //  goals and edges are.

    for(int i = 0; i < wallsSize; i++) {
        this->walls[i] = nullptr;
    }
}

void GameState::PutUnitInBuckets(Unit* unit) {
    this->buckets[unit->GetX() / MECHANICS_UNIT_SPACE_AMP_BY_ARENA]->Insert(unit);
}

void GameState::CreateUnit(int playerNum, int spawnValues, UnitType type) {
    bool spawns[MECHANICS_NUM_GOALS] = {
        (bool)(spawnValues & 1),
        (bool)(spawnValues & 2),
        (bool)(spawnValues & 4)
    };

    Goal* goal;
    Player* player;
    int unitX;
    int unitY;

    player = GetPlayer(playerNum);
    
    for(int i = 0; i < MECHANICS_NUM_GOALS; i++) {
        if(player->GetMoney() < MECHANICS_UNIT_PRICE) {
            break;
        }

        if(spawns[i]) {
            goal = GetGoal(playerNum, i + 1);
            if(goal == nullptr) {
                continue;
            }
            this->GoalToUnitStart(goal->GetX(), goal->GetY(), &unitX, &unitY);
            this->units[lastUnit] = new Unit(type, static_cast<PlayerColor>(playerNum), unitX, unitY);
            (this->lastUnit)++;
            player->SpendMoney(MECHANICS_UNIT_PRICE);
        }
    }
}

void GameState::CreateWall(int playerNum, int worldX, int worldY) {
    if(this->walls[worldY * MECHANICS_WORLD_STATE_WIDTH + worldX] == nullptr &&
       this->GetPlayerMoney(playerNum) > MECHANICS_WALL_PRICE) {

        this->GetPlayer(playerNum)->SpendMoney(MECHANICS_WALL_PRICE);
        this->walls[worldY * MECHANICS_WORLD_STATE_WIDTH + worldX] = new Wall();
        this->worldState->PutItem(worldX, worldY, WorldStateID::WALL);
        this->aiModule->UpdateAIWorld(AIUpdateType::ADD_WALL, 0, worldX, worldY);
    }
}

void GameState::CreateTower(int playerNum, int worldX, int worldY) {
    if((this->worldState->IsEmpty(worldX, worldY)) &&
        this->GetPlayerMoney(playerNum) > MECHANICS_TOWER_PRICE) {
        
        Player* thisPlayer = this->GetPlayer(playerNum);
        thisPlayer->SpendMoney(MECHANICS_TOWER_PRICE);

        WorldStateID id;
        int towerIndexAmplifier = 0;
        switch(playerNum) {
            case 1:
                id = WorldStateID::BLUE_TOWER;
                towerIndexAmplifier = 0;
                break;
            case 2:
                id = WorldStateID::RED_TOWER;
                towerIndexAmplifier = MECHANICS_MAX_TOWERS_PER_PLAYER;
                break;
            case 3:
                id = WorldStateID::GREEN_TOWER;
                towerIndexAmplifier = 2 * MECHANICS_MAX_TOWERS_PER_PLAYER;
                break;
            default:
                id = WorldStateID::YELLOW_TOWER;
                towerIndexAmplifier = 3 * MECHANICS_MAX_TOWERS_PER_PLAYER;
        }

        this->worldState->PutItem(worldX, worldY, id);
        this->aiModule->UpdateAIWorld(AIUpdateType::ADD_TOWER, playerNum, worldX, worldY);
        int index = towerIndexAmplifier;
        for(int i = 0; i < MECHANICS_MAX_TOWERS_PER_PLAYER; i++) {
            if(this->towers[index + i] == nullptr) {
                index += i;
                break;
            }
        }

        UnitDirection towerPriorityDirection = static_cast<UnitDirection>(thisPlayer->GetPlayerSide());

        this->towers[index] = new Tower(thisPlayer->GetColor(), index, worldX, worldY, towerPriorityDirection);
        thisPlayer->IncrementTowers();
    }
}

void GameState::RemoveTower(int playerNum, int towerId) {
    if(towers[towerId] == nullptr) {
        return;
    }

    this->GetPlayer(playerNum)->DecrementTowers();
   
    this->worldState->RemoveItem(towers[towerId]->GetX(), towers[towerId]->GetY());
    this->aiModule->UpdateAIWorld(AIUpdateType::REMOVE_TOWER, playerNum, towers[towerId]->GetX(), towers[towerId]->GetY());

    delete towers[towerId];
    towers[towerId] = nullptr;
}

void GameState::RemoveWall(int index) {
    if(walls[index] == nullptr) {
        return;
    }

    int x = index % MECHANICS_WORLD_STATE_WIDTH;
    int y = index / MECHANICS_WORLD_STATE_WIDTH;
    this->worldState->RemoveItem(x, y);
    this->aiModule->UpdateAIWorld(AIUpdateType::REMOVE_WALL, 1, x, y);

    delete walls[index];
    walls[index] = nullptr;
}

void GameState::RemoveUnit(int index) {
    delete this->units[index];
    (this->lastUnit)--;
    this->units[index] = this->units[lastUnit];
    this->units[lastUnit] = nullptr;
}

//world x and world y are in cell space
void GameState::ChangeTowerTarget(int towerId, int worldX, int worldY) {
    Tower* tower = towers[towerId];

    if(walls[worldY * MECHANICS_WORLD_STATE_WIDTH + worldX] != nullptr) {
        tower->SetTargetWall(walls[worldY * MECHANICS_WORLD_STATE_WIDTH + worldX]);
        tower->SetTargetType(TowerTargetType::WALL);
    } else {
        Tower* towerTarget = FindTowerByXY(worldX, worldY);
        if(towerTarget == nullptr) {
            return;
        }
        tower->SetTargetTower(towerTarget);
        tower->SetTargetType(TowerTargetType::TOWER);
    }

    tower->SetTargeting(true);
    tower->SetTargetX(worldX);
    tower->SetTargetY(worldY);
}

void GameState::TowerAction(Tower* tower) {
    if(tower->IsWaiting()) {
        tower->DecrementDelay();

        if(tower->GetDelay() <= 0) {
            Unit* targetUnit = nullptr;
            if(!this->TowerTargetExists(tower)) {
                targetUnit = this->FindUnitWithinTower(tower);
                if(targetUnit == nullptr) {
                    tower->DecrementDelay();
                    tower->SetTargetX(-1);
                    tower->SetTargetY(-1);
                    return;
                } else {
                    tower->SetTargetX(targetUnit->GetX() / MECHANICS_UNIT_SPACE_AMPLIFIER);
                    tower->SetTargetY(targetUnit->GetY() / MECHANICS_UNIT_SPACE_AMPLIFIER);
                }
            }

            this->TowerShoot(tower, targetUnit);
            tower->SetWaiting(false);
            tower->SetDelay(MECHANICS_TOWER_FIRING_LENGTH);
        }
    } else {
        tower->DecrementDelay();
        if(tower->GetDelay() <= 0) {
            tower->SetWaiting(true);
            tower->SetDelay(MECHANICS_TOWER_FIRING_DELAY);
        }
    }
}

void GameState::UpdateTowerHealth(Tower* tower) {
    tower->ShiftDelayedAttacks();
    if(tower->GetHealth() <= 0) {
        this->RemoveTower(static_cast<int>(tower->GetColor()), tower->GetTowerID());
    }
}

bool GameState::TowerTargetExists(Tower* tower) {
    if(!tower->IsTargeting()) {
        return false;
    }

    if(tower->GetTargetType() == TowerTargetType::TOWER) {
        Tower* tempTower = this->FindTowerByXY(tower->GetTargetX(), tower->GetTargetY());
        if(tempTower == nullptr) {
            tower->SetTargeting(false);
            tower->SetTargetTower(nullptr);
            return false;
        }
    } else {
        Wall* tempWall = this->walls[tower->GetTargetY() * MECHANICS_WORLD_STATE_WIDTH + tower->GetTargetX()];
        if(tempWall == nullptr) {
            tower->SetTargeting(false);
            tower->SetTargetWall(nullptr);
            return false;
        }
    }

    return true;
}

Unit* GameState::FindUnitWithinTower(Tower* tower) {
    //priority: sort by direction toward goal
    //  and closest to goal manhattan
    int startBucket = tower->GetX() - MECHANICS_TOWER_RADIUS;
    int endBucket = tower->GetX() + MECHANICS_TOWER_RADIUS;

    int towerX, towerY;
    this->FindTowerWorldCenter(tower->GetX(), tower->GetY(), &towerX, &towerY);

    Bucket* currBucket;
    Unit** bucketUnits;
    int bucketSize;

    int closestDistance = MECHANICS_UNIT_WORLD_BOTTOM_EXTREME;
    int tempDistance;
    bool prioritySet = false;
    Unit* targetUnit = nullptr;

    for(int i = startBucket; i <= endBucket; i++) {
        if(i < 0) {
            continue;
        } else if(i >= MECHANICS_WORLD_STATE_WIDTH) {
            break;
        }

        currBucket = this->buckets[i];
        bucketUnits = currBucket->GetUnits();
        bucketSize = currBucket->GetSize();

        for(int i = 0; i < bucketSize; i++) {
            if(bucketUnits[i]->GetColor() == tower->GetColor()) {
                continue;
            }

            if(this->WithinTowerRange(towerX, towerY, bucketUnits[i]->GetX(), bucketUnits[i]->GetY())) {
                if(tower->WithinPriorityDirection(bucketUnits[i]->GetTrajectory())) {
                    if(!prioritySet) {
                        closestDistance = this->GetDistance(bucketUnits[i], tower->GetPriorityDirection());
                        targetUnit = bucketUnits[i];
                        prioritySet = true;
                        continue;
                    }
                } else {
                    if(prioritySet) {
                        continue;
                    }
                }

                tempDistance = this->GetDistance(bucketUnits[i], tower->GetPriorityDirection());
                if(tempDistance < closestDistance) {
                    closestDistance = tempDistance;
                    targetUnit = bucketUnits[i];
                }
            }
        }
    }

    return targetUnit;
}

bool GameState::WithinTowerRange(int towerX, int towerY, int unitX, int unitY) {
    unitY = unitY / MECHANICS_UNIT_SPACE_AMPLIFIER;

    if((unitY < towerY - MECHANICS_TOWER_WORLD_RADIUS) ||
       (unitY > towerY + MECHANICS_TOWER_WORLD_RADIUS)) {
        return false;
    }

    unitX = unitX / MECHANICS_UNIT_SPACE_AMPLIFIER;

    if(((unitX - towerX) * (unitX - towerX) + 
        (unitY - towerY) * (unitY - towerY)) < 
        (MECHANICS_TOWER_WORLD_RADIUS * MECHANICS_TOWER_WORLD_RADIUS)) {
        return true;
    }

    return false;
}

void GameState::TowerShoot(Tower* tower, Unit* targetUnit) {
    if(targetUnit != nullptr) {
        targetUnit->AddDeathSentence();
        return;
    }

    if(tower->GetTargetType() == TowerTargetType::TOWER) {
        tower->GetTargetTower()->AddDelayedAttack();
    } else {
        tower->GetTargetWall()->AddDelayedAttack();
    }
}

void GameState::FindTowerWorldCenter(int cellX, int cellY, int* worldX, int* worldY) {
    *worldX = (cellX * GRAPHICS_UI_MAIN_ARENA_CELL_SIZE) + GRAPHICS_UI_MAIN_ARENA_HALF_CELL_SIZE;
    *worldY = (cellY * GRAPHICS_UI_MAIN_ARENA_CELL_SIZE) + GRAPHICS_UI_MAIN_ARENA_HALF_CELL_SIZE;
}

int GameState::GetDistance(Unit* unit, UnitDirection priorityDirection) {
    switch(priorityDirection) {
        case UnitDirection::UP:
            return unit->GetY();
        case UnitDirection::RIGHT:
            return MECHANICS_UNIT_WORLD_RIGHT_EXTREME - unit->GetX();
        case UnitDirection::DOWN:
            return MECHANICS_UNIT_WORLD_BOTTOM_EXTREME - unit->GetY();
        default:
            return unit->GetX();
    }
}

bool GameState::CheckCollide(Unit* unit1, Unit* unit2) {
    if(unit1->GetColor() == unit2->GetColor()) {
        return false;
    }

    if(unit1->GetHealth() <= 0 || unit2->GetHealth() <= 0) {
        return false;
    }

    int dx = std::abs(unit1->GetX() - unit2->GetX());
    if(dx > MECHANICS_UNIT_COLLISION_DISTANCE_UNIT_SPACE) {
        return false;
    }

    int dy = std::abs(unit1->GetY() - unit2->GetY());
    if(dy > MECHANICS_UNIT_COLLISION_DISTANCE_UNIT_SPACE) {
        return false;
    }

    dx /= MECHANICS_UNIT_SPACE_AMPLIFIER;
    dy /= MECHANICS_UNIT_SPACE_AMPLIFIER;

    if(dx*dx + dy*dy <= MECHANICS_UNIT_COLLISION_DISTANCE_SQUARED) {
        return true;
    }

    return false;
}

void GameState::DamageUnits(Unit* unit1, Unit* unit2) {
    if(unit1->GetType() == unit2->GetType()) {
        unit1->TakeDamage(MECHANICS_UNIT_STARTING_HEALTH);
        unit2->TakeDamage(MECHANICS_UNIT_STARTING_HEALTH);
        return;
    }

    int type1 = static_cast<int>(unit1->GetType());
    int type2 = static_cast<int>(unit2->GetType());

    if((type1 + 1) % 3 == type2) {
        unit1->TakeDamage(MECHANICS_UNIT_PARTIAL_DAMAGE);
        unit2->TakeDamage(MECHANICS_UNIT_STARTING_HEALTH);
        return;
    }

    if((type2 + 1) % 3 == type1) {
        unit1->TakeDamage(MECHANICS_GOAL_STARTING_HEALTH);
        unit2->TakeDamage(MECHANICS_UNIT_PARTIAL_DAMAGE);
    }
}
