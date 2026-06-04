#ifndef MECHANICS_GAME_OBJECTS_GOAL_H_
#define MECHANICS_GAME_OBJECTS_GOAL_H_

class Goal {
private:
    int playerNum;
    int health;
    int x;
    int y;
    int spawnNum;
public:
    Goal(int playerNum, int x, int y, int spawnNum);
    ~Goal();

    int GetPlayerNum();

    int GetHealth();
    void DecrementHealth();

    int GetX();
    int GetY();
    int GetSpawnNum();
};

#endif