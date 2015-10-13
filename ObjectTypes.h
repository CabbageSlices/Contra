#ifndef OBJECTTYPES_H_INCLUDED
#define OBJECTTYPES_H_INCLUDED

enum EnemyType : int {

    ENEMY_GOOMBA = 0,
    ENEMY_PIRANHA,
    ENEMY_MUSHROOM,
};

enum BulletType : int {

    BULLET_SLOW = 0,
    BULLET_FAST,
    BULLET_MEDIUM,
};

enum DestructibleBlockType : int {

    BLOCK_BRICK = 0,
};

enum GunType : int{

    GUN_BASIC = 0
};

enum PowerUpType : int{

    MACHINE_GUN = 0
};

#endif // OBJECTTYPES_H_INCLUDED
