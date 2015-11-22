#ifndef STATEINTROEFFECTS_H_INCLUDED
#define STATEINTROEFFECTS_H_INCLUDED

#include "StateTransitionEffect.h"

//effect has a black cover ontop of the screen, which then moves off screen towards the right, and reveals the current game state
class BlackCoverWipeRightIntro : public BlackCoverWipe {

    public:

        BlackCoverWipeRightIntro();
        virtual ~BlackCoverWipeRightIntro(){}

        virtual void begin();
        virtual bool isFinished();
};

#endif // STATEINTROEFFECTS_H_INCLUDED
