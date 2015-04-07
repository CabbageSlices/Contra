#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "Player.h"
#include "Tile.h"
#include "TileMap.h"

int main() {

    sf::RenderWindow window(sf::VideoMode(1024, 768), "Contra");

    sf::Event event;

    Player player;

    sf::Clock timer;

    TileMap tileMap(1024, 768);

    while(window.isOpen()) {

        while(window.pollEvent(event)) {

            if(event.type == sf::Event::Closed) {

                window.close();
            }

            if(event.type == sf::Event::KeyPressed) {

                if(event.key.code == sf::Keyboard::Escape) {

                    window.close();
                }
            }

            player.handleInputEvents(event, window);
        }

        player.handleKeystate(window);

        sf::Time deltaTime = timer.restart();

        player.update(deltaTime.asSeconds(), sf::FloatRect(0, 0, 1024, 768));

        window.clear();

        tileMap.drawDebug(window, glm::vec2(0, 0), glm::vec2(1024, 768));
        player.draw(window);
        window.display();
    }
    return 0;
}
