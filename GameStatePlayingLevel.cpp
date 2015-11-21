#include "GameStatePlayingLevel.h"
#include "WorldSavingLoading.h"
#include "PlayingLevelFunctions.h"
#include "StateManager.h"
#include "GameStateLevelIntro.h"
#include "GameStateOutro.h"
#include <cstdlib>

using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;

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

        //camera should be updated that way if this state is created and another state draws on top of this one, the camera is correctly setup to draw the world
        vector<glm::vec2> playerPositions;
        playerPositions.push_back(world.initialPlayerSpawnPoint);

        updateCameraProperties(world, playerPositions, 0);
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

    if(world.shouldBeginBossFight()) {

        world.beginBossFight();
    }

    updateWorldPhyics(world, deltaTime);
	updateEnemySpawners(world, world.nonBossEnemySpawners);

	//don't update boss spawners unless player is actulaly fighting bosses
	if(world.worldState == GameWorld::FIGHTING_BOSS) {

        updateEnemySpawners(world, world.bossEnemySpawners, true);
	}

	if(allBossesDefeated()) {

        goToNextLevel(window, stateManager);
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

bool GameStatePlayingLevel::allBossesDefeated() {

    if(world.worldState != GameWorld::FIGHTING_BOSS) {

        //can't defeat all the bosses without even starting the fight
        return false;
    }

    //currently fighting boss, if no more bosses left to spawn and all bosses defeated, then transition to next level
    if(canSpawnEnemy(world.bossEnemySpawners)) {

        //still bosses left to spawn
        return false;
    }

    //no bosses left to spawn, check if any bosses are being fought
    if(enemiesRemaining(world.bossEnemyCollection)) {

        return false;
    }

    return true;
}

void GameStatePlayingLevel::goToNextLevel(sf::RenderWindow &window, StateManager &stateManager) {

    //create the next level
    shared_ptr<GameState> nextLevel = make_shared<GameStatePlayingLevel>("world1", world.players, window);

    //intro transition to next level
    shared_ptr<GameState> nextLevelIntro = make_shared<GameStateLevelIntro>(window);

    vector<State> statesFollowingOutro;
    statesFollowingOutro.push_back(nextLevel);
    statesFollowingOutro.push_back(nextLevelIntro);

    //outro to this level
    shared_ptr<GameState> outro = make_shared<GameStateOutro>(window, true, statesFollowingOutro);

    //insert the outro, once outro is finished it will automatically get rid of this level, and add the new level
    stateManager.push(outro);
}
