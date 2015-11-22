#ifndef STATEOUTROEFFECTS_H_INCLUDED
#define STATEOUTROEFFECTS_H_INCLUDED

#include "StateTransitionEffect.h"

class BlackCoverWipeRightOutro : public BlackCoverWipe {

    public:

        BlackCoverWipeRightOutro();
        virtual ~BlackCoverWipeRightOutro(){}

        virtual void begin();
        virtual bool isFinished();
};

#endif // STATEOUTROEFFECTS_H_INCLUDED
