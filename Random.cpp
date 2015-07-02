#include "Random.h"

#include <iostream>

using std::cout;
using std::endl;

int getRand(const int &min, const int &max) {

    if(min >= max) {

        return max;
    }

    //if min == 0 then the range given by rand % max is [0, max)
    //else the range is [min, max + min), but we want [min, max]
    //so first reduce the range to [0, max - 1]
    int newMax = max - min;

    //rand() % x returns a number in the range [0, x - 1], so increase x by 1 before doing the modulo that way the range is [0, x]
    int random = rand() % (newMax + 1);

    //add min to the random number that way its in the range [min, max]
    return random + min;
}

//gets a random number [1, possibleOutcomes] and if the number is less than occurance threshold then the event occurs
bool determineIfEventOccurs(const unsigned &occuranceThreshold, const unsigned &possibleOutcomes) {

    int random = getRand(1, possibleOutcomes);
    cout << random << endl;

    return random <= occuranceThreshold;
}
