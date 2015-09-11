#ifndef SAVEDDATATAGS_H_INCLUDED
#define SAVEDDATATAGS_H_INCLUDED

#include <utility>
#include <string>

//every piece of data saved has a tag
//this tagpair stores the begin and end tag for some type of data
//first in the pair is the tag at the beginning of the data block, and the second item is the ending tag.
typedef std::pair<std::string, std::string> DataTagPair;

extern const DataTagPair playerSpawnTag;
extern const DataTagPair tileMapTag;
extern const DataTagPair backgroundTag;
extern const DataTagPair worldBoundsTag;
extern const DataTagPair worldBoundsBossFightTag;
extern const DataTagPair nonBossEnemySpawnerTag;
extern const DataTagPair bossEnemySpawnerTag;
extern const DataTagPair basicEnemySpawnerTag;
extern const DataTagPair turretEnemySpawnerTag;
extern const DataTagPair omnidirectionalTurretSpawnerTag;
extern const DataTagPair destructibleBlocksTag;

extern const DataTagPair playerControlsTag;

#endif // SAVEDDATATAGS_H_INCLUDED
