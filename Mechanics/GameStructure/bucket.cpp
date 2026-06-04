#include "bucket.h"

Bucket::Bucket() {
    this->units = new Unit*[MECHANICS_GAME_STATE_MAX_UNITS_PER_BUCKET];
}

//Deletes just the array
Bucket::~Bucket() {
    delete[] this->units;
}

void Bucket::Insert(Unit* unit) {
    this->units[this->lastUnit] = unit;
    (this->lastUnit)++;
}

void Bucket::Clear() {
    this->lastUnit = 0;
}

Unit** Bucket::GetUnits() {
    return this->units;
}

int Bucket::GetSize() {
    return this->lastUnit;
}
