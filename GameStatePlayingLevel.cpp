#include "GameStatePlayingLevel.h"
#include "WorldSavingLoading.h"
#include "PlayingLevelFunctions.h"
#include <cstdlib>

using std::string;
using std::vector;
using std::shared_ptr;

GameStatePlayingLevel::GameStatePlayingLevel(const string &levelToLoad, vector<shared_ptr<Player> > &players, sf::RenderWindow &window) :
    GameState(false),
    world(window),
    levelName(levelToLoad)
    {
        world.players.insert(world.players.end(), players.begin(), players.end());

        //when the level starts, the players should all be spawned at their initial position
        for(auto &player : world.players) {

            player->spawn(world.initialPlayerSpawnPoint);
        }

        //do some level loading
        //what if levle loading fails? i dunno, throwing exceptions aren't a good idea in constructors or destructors
        //most likely you should just crash the game
        loadWorld(levelToLoad, world);
    }

void GameStatePlayingLevel::handleInputEvents(sf::Event &event, sf::RenderWindow &window, StateManager& stateManager) {

    if(event.type == sf::Event::Resized) {

        world.camera.setupDefaultProperties(window);
    }

    if(world.worldState != GameWorld::TRANSITIONING_TO_BOSS_FIGHT) {

        for(unsigned i = 0; i < world.players.size(); ++i) {

            world.players[i]->handleInputEvents(event, window);
        }
    }
}

void GameStatePlayingLevel::handleStateEvents(sf::RenderWindow &window, StateManager &stateManager) {

    if(world.worldState != GameWorld::TRANSITIONING_TO_BOSS_FIGHT) {

        for(unsigned i = 0; i < world.players.size(); ++i) {

            world.players[i]->handleKeystate(window);
        }
    }
}

void GameStatePlayingLevel::update(float deltaTime, sf::RenderWindow &window, StateManager &stateManager) {

    //determine current state of the gameworld
    if(world.worldState == GameWorld::NOT_FIGHTING_BOSS && world.shouldBeginBossFightTransition()) {

        world.beginBossFightTransition();
    }

    if(world.worldState == GameWorld::TRANSITIONING_TO_BOSS_FIGHT && world.shouldBeginBossFight()) {

        world.beginBossFight();
    }

    updateWorldPhyics(world, deltaTime);
	updateEnemySpawners(world, world.nonBossEnemySpawners);

	//don't update boss spawners unless player is actulaly fighting bosses
	if(world.worldState == GameWorld::FIGHTING_BOSS) {

        updateEnemySpawners(world, world.bossEnemySpawners, true);
	}

	handleEntityCollisions(world);
	updateWorldRendering(window, world);
}

void GameStatePlayingLevel::draw(sf::RenderWindow &window, StateManager &stateManager) {

    //apply camera before drawing world
    world.camera.applyCamera(window);

    world.backgrounds.draw(window);
	drawTiles(window, world);
	drawEntities(window, world.players);
	drawEnemyCollection(window, world.nonBossEnemyCollection);
	drawEnemyCollection(window, world.bossEnemyCollection);
	drawEntities(window, world.destructibleBlocks);
	drawEntities(window, world.powerUps);
}
