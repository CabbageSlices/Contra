#ifndef GLOBALCONSTANTS_H_INCLUDED
#define GLOBALCONSTANTS_H_INCLUDED

///define constants that can be used at any point in the program
///consists only of constants that are required for math or physics

extern const float PI;
extern const float DEG_TO_RAD_RATIO;
extern const float RAD_TO_DEG_RATIO;

//physics constants
///uses kilograms, meters, and seconds for units
extern const float PIXELS_TO_METER_RATIO;
extern const float METERS_TO_PIXEL_RATIO;

//max velocity for most objects
//only used so objects can use this for reference
//there is no guarantee that objects will use this as an upper limit
//meters/second
extern const float TERMINAL_VELOCITY;

//meters per squared second
extern const float GRAVITY;

#endif // GLOBALCONSTANTS_H_INCLUDED
