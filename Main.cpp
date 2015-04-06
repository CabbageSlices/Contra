#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "Player.h"
#include "Tile.h"

int main() {

    sf::RenderWindow window(sf::VideoMode(1024, 768), "Contra");

    sf::Event event;

    Player player;

    sf::Clock timer;

    Tile tile(glm::vec2(0, 704), TileType::ONE_WAY);

    while(window.isOpen()) {

        while(window.pollEvent(event)) {

            if(event.type == sf::Event::Closed) {

                window.close();
            }

            player.handleInputEvents(event, window);
        }

        player.handleKeystate(window);

        sf::Time deltaTime = timer.restart();

        player.update(deltaTime.asSeconds(), sf::FloatRect(0, 0, 1024, 768), tile);

        window.clear();

        tile.drawDebug(window);
        player.draw(window);
        window.display();
    }
    return 0;
}
