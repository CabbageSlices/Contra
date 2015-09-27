#ifndef ENTITYANIMATIONSTATES_H_INCLUDED
#define ENTITYANIMATIONSTATES_H_INCLUDED

namespace PlayerEnums {

    enum PlayerAnimationStates : unsigned {

        //all possible animation states for the player
        //when it says up/down facing left/right it means that player is looking straight up or down, but his body is still facing toward the left or right
        //it doesn't mean he is movign
        STATE_STANDING_LEFT = 0,
        STATE_STANDING_UP_FACING_LEFT,
        STATE_STANDING_UP_FACING_RIGHT,
        STATE_STANDING_RIGHT,

        STATE_WALKING_DOWN_LEFT,
        STATE_WALKING_LEFT,
        STATE_WALKING_UP_LEFT,
        STATE_WALKING_UP_RIGHT,
        STATE_WALKING_RIGHT,
        STATE_WALKING_DOWN_RIGHT,

        STATE_CROUCHING_LEFT,
        STATE_CROUCHING_RIGHT,

        STATE_FALLING_DOWN_FACING_LEFT,
        STATE_FALLING_DOWN_FACING_RIGHT,
        STATE_FALLING_DOWN_LEFT,
        STATE_FALLING_LEFT,
        STATE_FALLING_UP_LEFT,
        STATE_FALLING_UP_FACING_LEFT,
        STATE_FALLING_UP_FACING_RIGHT,
        STATE_FALLING_UP_RIGHT,
        STATE_FALLING_RIGHT,
        STATE_FALLING_DOWN_RIGHT,

        STATE_JUMPING,

        STATE_DYING_FACING_RIGHT,
        STATE_DYING_FACING_LEFT,

        STATE_DEAD,
    };
}



namespace TurretEnemyEnums {

    enum TurretEnemyAnimationStates : unsigned {

        STATE_HIDING = 0,
        STATE_COMING_OUT_OF_HIDING,
        STATE_GOING_INTO_HIDING,
        STATE_SHOOTING,
    };

    enum TurretEnemyShootingDirectionTextureRects : unsigned {

        //texture rect index number for each direction the object faces when shooting
        //when turret enemy is shooting it can shoot in 8 different directions
        //instead of having each direction be a different animation state,
        //i decided to make each direction correspond to a single frame of the shooting animation
        //each direction listed below corresponds to a frame of animation
        DOWN = 0,
        DOWN_LEFT,
        LEFT,
        UP_LEFT,
        UP,
        UP_RIGHT,
        RIGHT,
        DOWN_RIGHT,
    };
}

namespace OmniDirectionalTurretEnums{

    enum OmniDirectionalTurretAnimationStates : unsigned {

        STATE_HIDING = 0,
        STATE_COMING_OUT_OF_HIDING,
        STATE_GOING_INTO_HIDING,
        STATE_EXPOSED,
        STATE_SHOOTING,
    };

}

namespace EnemyEnums {

    enum EnemyAnimationStates : unsigned {

        STATE_WALKING_LEFT = 0,
        STATE_WALKING_RIGHT,
        STATE_FALLING_LEFT,
        STATE_FALLING_RIGHT,
    };
}

namespace DestructibleBlockEnums {

    enum DestructibleBlockAnimationStates : unsigned {

        STATE_SOLID = 0,
        STATE_DESTROYING,
        STATE_DESTROYED,
    };
}

namespace BulletEnums {

    enum BulletAnimationStates : unsigned {

        STATE_RIGHT = 0,
        STATE_UP_RIGHT,
        STATE_UP,
        STATE_UP_LEFT,
        STATE_LEFT,
        STATE_DOWN_LEFT,
        STATE_DOWN,
        STATE_DOWN_RIGHT,
    };
}

namespace PowerUpEnums {

    enum PowerUpAnimationStates : unsigned {

        STATE_SPAWNING = 0,
        STATE_SPAWNED,
        STATE_DISAPPEARING,
        STATE_DISAPPEARED,
    };

    enum PowerUpTextureRects : unsigned {

        //after a powerup is spawned it needs to display an icon to indicate what sort of powerup it is
        //instead of having seperate powerup animations for each type of powerup,
        //i chose to have each frame of the powerup's spawned animation refer to a different powerup
        //each of tehse enums refer to a frame of the spawned animation, and will display a single powerup type
        FRAME_MACHINEGUN = 0,
    };
}

#endif // ENTITYANIMATIONSTATES_H_INCLUDED
