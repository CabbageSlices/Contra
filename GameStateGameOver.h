#ifndef GAMESTATEGAMEOVER_H_INCLUDED
#define GAMESTATEGAMEOVER_H_INCLUDED

#include "GameStateBase.h"
#include "ImageUIComponent.h"

//game over state, once the players all die this screen should show up
//this might have some effect of slowly fading in, so the previous state will be drawn
//afterwards it has to pop the previous state, which is just the level, and then move to some other state, probably the main menu
//however if you want to transition into the main menu, you might have to make it so the main menu is popped at the very beginning, when it goes to gameplay
//this way when you return to main menu you dont have to keep track of it's position in the state stack, and can instead push a new menu state
class GameStateGameOver : public GameStateBase {

    public:

        GameStateGameOver()

    private:

        //game over image
        ImageUIComponent gameOverImage;
};

#endif // GAMESTATEGAMEOVER_H_INCLUDED
