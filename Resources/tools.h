#ifndef RESOURCES_TOOLS_H_
#define RESOURCES_TOOLS_H_

#include <cstdlib>
#include <ctime>
#include <utility>
#include "..\config.h"

class Tools {
private:
    Tools();
    static bool initialized;
public:
    template <typename T>
    static void ShuffleArray(T* array, int size) {
        if(!Tools::initialized) {
            srand(static_cast<unsigned int>(time(nullptr)));
        }

        int index1, index2;
        T tmpValue;
        for(int i = 0; i < ARRAY_SHUFFLE_AMOUNT; i++) {
            index1 = rand() % size;
            index2 = rand() % size;

            if(index1 == index2) {
                continue;
            }

            tmpValue = array[index1];
            array[index1] = array[index2];
            array[index2] = tmpValue;
        }
    }
};

#endif