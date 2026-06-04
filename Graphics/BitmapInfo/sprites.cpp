#include <unordered_map>
#include <iostream>

#include "sprites.h"
#include "..\..\config.h"

std::unordered_map<BitmapID, std::string, BitmapIDHash> Sprites::fileMappings = {
    {BitmapID::BLUE_CIRCLE_BUTTON, "blueCircleButton.ppm"},
    {BitmapID::BLUE_CIRCLE_PRESSED, "blueCirclePressed.ppm"},
    {BitmapID::BLUE_CIRCLE_UNIT, "blueCircleUnit.ppm"},
    {BitmapID::BLUE_GOAL, "blueGoal.ppm"},
    {BitmapID::BLUE_GOAL_SELECTED, "blueGoalSelected.ppm"},
    {BitmapID::BLUE_SQUARE_BUTTON, "blueSquareButton.ppm"},
    {BitmapID::BLUE_SQUARE_PRESSED, "blueSquarePressed.ppm"},
    {BitmapID::BLUE_SQUARE_UNIT, "blueSquareUnit.ppm"},
    {BitmapID::BLUE_TOWER_BUTTON, "blueTowerButton.ppm"},
    {BitmapID::BLUE_TOWER_PRESSED, "blueTowerPressed.ppm"},
    {BitmapID::BLUE_TRIANGLE_BUTTON, "blueTriangleButton.ppm"},
    {BitmapID::BLUE_TRIANGLE_PRESSED, "blueTrianglePressed.ppm"},
    {BitmapID::BLUE_TRIANGLE_UNIT, "blueTriangleUnit.ppm"},
    {BitmapID::BOLT, "bolt.ppm"},
    {BitmapID::DEAD_GOAL, "deadGoal.ppm"},
    {BitmapID::EDGE_BOTTOM_LEFT, "edgeBottomLeft.ppm"},
    {BitmapID::EDGE_BOTTOM_RIGHT, "edgeBottomRight.ppm"},
    {BitmapID::EDGE_GOAL_BOTTOM, "edgeGoalBottom.ppm"},
    {BitmapID::EDGE_GOAL_LEFT, "edgeGoalLeft.ppm"},
    {BitmapID::EDGE_GOAL_RIGHT, "edgeGoalRight.ppm"},
    {BitmapID::EDGE_GOAL_TOP, "edgeGoalTop.ppm"},
    {BitmapID::EDGE_HORIZONTAL, "edgeHorizontal.ppm"},
    {BitmapID::EDGE_TOP_LEFT, "edgeTopLeft.ppm"},
    {BitmapID::EDGE_TOP_RIGHT, "edgeTopRight.ppm"},
    {BitmapID::EDGE_VERTICAL, "edgeVertical.ppm"},
    {BitmapID::EMPTY_TILE_BOTH, "emptyTileBoth.ppm"},
    {BitmapID::EMPTY_TILE_BOTTOM, "emptyTileBottom.ppm"},
    {BitmapID::EMPTY_TILE_NEITHER, "emptyTileNeither.ppm"},
    {BitmapID::EMPTY_TILE_RIGHT, "emptyTileRight.ppm"},
    {BitmapID::GREEN_CIRCLE_BUTTON, "greenCircleButton.ppm"},
    {BitmapID::GREEN_CIRCLE_PRESSED, "greenCirclePressed.ppm"},
    {BitmapID::GREEN_CIRCLE_UNIT, "greenCircleUnit.ppm"},
    {BitmapID::GREEN_GOAL, "greenGoal.ppm"},
    {BitmapID::GREEN_GOAL_SELECTED, "greenGoalSelected.ppm"},
    {BitmapID::GREEN_SQUARE_BUTTON, "greenSquareButton.ppm"},
    {BitmapID::GREEN_SQUARE_PRESSED, "greenSquarePressed.ppm"},
    {BitmapID::GREEN_SQUARE_UNIT, "greenSquareUnit.ppm"},
    {BitmapID::GREEN_TOWER_BUTTON, "greenTowerButton.ppm"},
    {BitmapID::GREEN_TOWER_PRESSED, "greenTowerPressed.ppm"},
    {BitmapID::GREEN_TRIANGLE_BUTTON, "greenTriangleButton.ppm"},
    {BitmapID::GREEN_TRIANGLE_PRESSED, "greenTrianglePressed.ppm"},
    {BitmapID::GREEN_TRIANGLE_UNIT, "greenTriangleUnit.ppm"},
    {BitmapID::LOADING_EMPTY, "loadingEmpty.ppm"},
    {BitmapID::LOADING_FILLED, "loadingFilled.ppm"},
    {BitmapID::MAIN_BACKGROUND, "mainBackground.ppm"},
    {BitmapID::MAIN_X_BUTTON, "mainXButton.ppm"},
    {BitmapID::MAIN_X_PRESSED, "mainXPressed.ppm"},
    {BitmapID::MONEY_EMPTY, "moneyEmpty.ppm"},
    {BitmapID::MONEY_FILLED, "moneyFilled.ppm"},
    {BitmapID::RED_CIRCLE_BUTTON, "redCircleButton.ppm"},
    {BitmapID::RED_CIRCLE_PRESSED, "redCirclePressed.ppm"},
    {BitmapID::RED_CIRCLE_UNIT, "redCircleUnit.ppm"},
    {BitmapID::RED_GOAL, "redGoal.ppm"},
    {BitmapID::RED_GOAL_SELECTED, "redGoalSelected.ppm"},
    {BitmapID::RED_SQUARE_BUTTON, "redSquareButton.ppm"},
    {BitmapID::RED_SQUARE_PRESSED, "redSquarePressed.ppm"},
    {BitmapID::RED_SQUARE_UNIT, "redSquareUnit.ppm"},
    {BitmapID::RED_TOWER_BUTTON, "redTowerButton.ppm"},
    {BitmapID::RED_TOWER_PRESSED, "redTowerPressed.ppm"},
    {BitmapID::RED_TRIANGLE_BUTTON, "redTriangleButton.ppm"},
    {BitmapID::RED_TRIANGLE_PRESSED, "redTrianglePressed.ppm"},
    {BitmapID::RED_TRIANGLE_UNIT, "redTriangleUnit.ppm"},
    {BitmapID::TOWER_BLUE, "towerBlue.ppm"},
    {BitmapID::TOWER_GREEN, "towerGreen.ppm"},
    {BitmapID::TOWER_RADIUS, "towerRadius.ppm"},
    {BitmapID::TOWER_RED, "towerRed.ppm"},
    {BitmapID::TOWER_YELLOW, "towerYellow.ppm"},
    {BitmapID::UI_BACK_BUTTON, "uiBackButton.ppm"},
    {BitmapID::UI_BACKGROUND, "uiBackground.ppm"},
    {BitmapID::UI_BACK_PRESSED, "uiBackPressed.ppm"},
    {BitmapID::UI_BUTTON_PRESSED_TEMPLATE, "uiButtonPressedTemplate.ppm"},
    {BitmapID::UI_BUTTON_TEMPLATE, "uiButtonTemplate.ppm"},
    {BitmapID::UI_CONTINUE_BUTTON, "uiContinueButton.ppm"},
    {BitmapID::UI_CONTINUE_PRESSED, "uiContinuePressed.ppm"},
    {BitmapID::UI_HUMAN_BUTTON, "uiHumanButton.ppm"},
    {BitmapID::UI_HUMAN_EMPTY, "uiHumanEmpty.ppm"},
    {BitmapID::UI_HUMAN_FULL, "uiHumanFull.ppm"},
    {BitmapID::UI_HUMAN_PRESSED, "uiHumanPressed.ppm"},
    {BitmapID::UI_ROBOT, "uiRobot.ppm"},
    {BitmapID::UI_ROBOT_PRESSED, "uiRobotPressed.ppm"},
    {BitmapID::UI_X_BUTTON, "uiXButton.ppm"},
    {BitmapID::UI_X_PRESSED, "uiXPressed.ppm"},
    {BitmapID::UNAVAILABLE_CIRCLE, "unavailableCircle.ppm"},
    {BitmapID::UNAVAILABLE_SQUARE_BUTTON, "unavailableSquareButton.ppm"},
    {BitmapID::UNAVAILABLE_TOWER, "unavailableTower.ppm"},
    {BitmapID::UNAVAILABLE_TRIANGLE_BUTTON, "unavailableTriangleButton.ppm"},
    {BitmapID::WALL_BUTTON, "wallButton.ppm"},
    {BitmapID::WALL_PRESSED, "wallPressed.ppm"},
    {BitmapID::WALL_UNAVAILABLE, "wallUnavailable.ppm"},
    {BitmapID::WOOD_WALL, "woodWall.ppm"},
    {BitmapID::WOOD_WALL_CURSOR_UNAVAILABLE, "woodWallCursorUnavailable.ppm"},
    {BitmapID::WOOD_WALL_UNPLACE, "woodWallUnplace.ppm"},
    {BitmapID::YELLOW_CIRCLE_BUTTON, "yellowCircleButton.ppm"},
    {BitmapID::YELLOW_CIRCLE_PRESSED, "yellowCirclePressed.ppm"},
    {BitmapID::YELLOW_CIRCLE_UNIT, "yellowCircleUnit.ppm"},
    {BitmapID::YELLOW_GOAL, "yellowGoal.ppm"},
    {BitmapID::YELLOW_GOAL_SELECTED, "yellowGoalSelected.ppm"},
    {BitmapID::YELLOW_SQUARE_BUTTON, "yellowSquareButton.ppm"},
    {BitmapID::YELLOW_SQUARE_PRESSED, "yellowSquarePressed.ppm"},
    {BitmapID::YELLOW_SQUARE_UNIT, "yellowSquareUnit.ppm"},
    {BitmapID::YELLOW_TOWER_BUTTON, "yellowTowerButton.ppm"},
    {BitmapID::YELLOW_TOWER_PRESSED, "yellowTowerPressed.ppm"},
    {BitmapID::YELLOW_TRIANGLE_BUTTON, "yellowTriangleButton.ppm"},
    {BitmapID::YELLOW_TRIANGLE_PRESSED, "yellowTrianglePressed.ppm"},
    {BitmapID::YELLOW_TRIANGLE_UNIT, "yellowTriangleUnit.ppm"}
};

std::unordered_map<WorldStateID, BitmapID, WorldStateIDHash> Sprites::worldMappings = {
    {WorldStateID::GOAL_BLUE, BitmapID::BLUE_GOAL},
    {WorldStateID::DEAD_GOAL, BitmapID::DEAD_GOAL},    
    {WorldStateID::EDGE_CORNER_BOTTOM_LEFT, BitmapID::EDGE_BOTTOM_LEFT},
    {WorldStateID::EDGE_CORNER_BOTTOM_RIGHT, BitmapID::EDGE_BOTTOM_RIGHT},
    {WorldStateID::EDGE_GOAL_BOTTOM, BitmapID::EDGE_GOAL_BOTTOM},
    {WorldStateID::EDGE_GOAL_LEFT, BitmapID::EDGE_GOAL_LEFT},
    {WorldStateID::EDGE_GOAL_RIGHT, BitmapID::EDGE_GOAL_RIGHT},
    {WorldStateID::EDGE_GOAL_TOP, BitmapID::EDGE_GOAL_TOP},
    {WorldStateID::EDGE_HORIZONTAL, BitmapID::EDGE_HORIZONTAL},
    {WorldStateID::EDGE_CORNER_TOP_LEFT, BitmapID::EDGE_TOP_LEFT},
    {WorldStateID::EDGE_CORNER_TOP_RIGHT, BitmapID::EDGE_TOP_RIGHT},
    {WorldStateID::EDGE_VERTICAL, BitmapID::EDGE_VERTICAL},
    {WorldStateID::EMPTY_BOTH, BitmapID::EMPTY_TILE_BOTH},
    {WorldStateID::EMPTY_BOTTOM, BitmapID::EMPTY_TILE_BOTTOM},
    {WorldStateID::EMPTY_NEITHER, BitmapID::EMPTY_TILE_NEITHER},
    {WorldStateID::EMPTY_RIGHT, BitmapID::EMPTY_TILE_RIGHT},
    {WorldStateID::GOAL_GREEN, BitmapID::GREEN_GOAL},
    {WorldStateID::GOAL_RED, BitmapID::RED_GOAL},
    {WorldStateID::BLUE_TOWER, BitmapID::TOWER_BLUE},
    {WorldStateID::GREEN_TOWER, BitmapID::TOWER_GREEN},
    {WorldStateID::RED_TOWER, BitmapID::TOWER_RED},
    {WorldStateID::YELLOW_TOWER, BitmapID::TOWER_YELLOW},
    {WorldStateID::WALL, BitmapID::WOOD_WALL},
    {WorldStateID::GOAL_YELLOW, BitmapID::YELLOW_GOAL}
};


std::string Sprites::EncodingToFilename(BitmapID id) {
    std::unordered_map<BitmapID, std::string, BitmapIDHash>::iterator it = Sprites::fileMappings.find(id);

    if(it != Sprites::fileMappings.end()) {
        return it->second; 
    }

    std::cout << "Encoding " << static_cast<size_t>(id) << " not in file mappings." << std::endl;

    return "";
}

BitmapID Sprites::WorldStateIDToBitmapID(WorldStateID id) {
    std::unordered_map<WorldStateID, BitmapID, WorldStateIDHash>::iterator it = Sprites::worldMappings.find(id);

    if(it != Sprites::worldMappings.end()) {
        return it->second; 
    }

    std::cout << "Encoding " << static_cast<size_t>(id) << " not in world mappings." << std::endl;

    return BitmapID::UI_BUTTON_TEMPLATE;
}