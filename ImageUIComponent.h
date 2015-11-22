#ifndef IMAGEUICOMPONENT_H_INCLUDED
#define IMAGEUICOMPONENT_H_INCLUDED

#include "UserInterfaceComponent.h"
#include <string>
#include <map>

//image ui component are UI components that are images, go figure
//it displays an image onto the screen which will be displayed regardless of the current view, like all other UI components
class ImageUIComponent : public UserInterfaceComponent {

    public:

        ImageUIComponent(const sf::Vector2f &defaultScreenRes, const std::string &imagePath, const sf::Vector2f &topLeftPosition);
        virtual ~ImageUIComponent(){}

        virtual void handleScreenResize(const sf::Vector2f &newScreenResolution);

    private:

        virtual void drawInternal(sf::RenderTarget &target);
        void loadTexture(const std::string &texturePath);

        sf::Sprite sprite;

        static std::map<std::string, sf::Texture> textures;
};

#endif // IMAGEUICOMPONENT_H_INCLUDED
