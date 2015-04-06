#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "Player.h"

int main() {

    sf::RenderWindow window(sf::VideoMode(1024, 768), "Contra");

    sf::Event event;

    Player player;

    sf::Clock timer;

    while(window.isOpen()) {

        while(window.pollEvent(event)) {

            if(event.type == sf::Event::Closed) {

                window.close();
            }

            player.handleInputEvents(event, window);
        }

        player.handleKeystate(window);

        sf::Time deltaTime = timer.restart();

        player.update(deltaTime.asSeconds(), sf::FloatRect(0, 0, 1024, 768));

        window.clear();

        player.draw(window);
        window.display();
    }
    return 0;
}
