#ifndef MECHANICS_GAME_STRUCTURE_BUCKET_H_
#define MECHANICS_GAME_STRUCTURE_BUCKET_H_

#include "..\GameObjects\unit.h"

class Bucket {
public:
private:
    Unit** units = nullptr;
    int lastUnit = 0;
public:
    Bucket();
    ~Bucket();

    void Insert(Unit* unit);
    void Remove(int index);
    void Clear();

    Unit** GetUnits();
    int GetSize();
private:
};

#endif