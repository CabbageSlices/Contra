#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

#include <cstdlib>

//gets a random number in the range [min, max]
int getRand(const int &min, const int &max);

//gets a random number [1, possibleOutcomes] and if the number is less than occurance threshold then the event occurs
bool determineIfEventOccurs(const unsigned &occuranceThreshold, const unsigned &possibleOutcomes);

#endif // RANDOM_H_INCLUDED
