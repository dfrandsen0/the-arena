#include "serverConnect.h"
#include "..\config.h"

ServerConnect::ServerConnect() {
    //empty, private, prevents init
}

std::pair<int, int>* ServerConnect::GetSidesDummy(int* thisPlayer) {
    std::pair<int, int>* pairings = new std::pair<int, int>[MECHANICS_MAX_NUM_PLAYERS];
    for(int i = 0; i < MECHANICS_MAX_NUM_PLAYERS; i++) {
        pairings[i].first = i + 1;
        pairings[i].second = i;
    }

    (*thisPlayer) = pairings[1].first;

    return pairings;
}