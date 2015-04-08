#include "ObjectSpaceManager.h"
#include <iostream>

using std::cout;
using std::endl;

ObjectSpaceManager::ObjectSpaceManager(const glm::vec2& xAxis, const glm::vec2& yAxis, const glm::vec2& sizeObjectSpace):
    objectToWorldSpace(xAxis, yAxis),
    worldToObjectSpace(glm::inverse(objectToWorldSpace)),
    positionInObjectSpace(0, 0),
    sizeInObjectSpace(sizeObjectSpace)
    {

    }

glm::vec2 ObjectSpaceManager::convertToObjectSpace(const glm::vec2& worldSpacePosition) const {

    return worldToObjectSpace * worldSpacePosition;
}

sf::FloatRect ObjectSpaceManager::convertToObjectSpace(const sf::FloatRect& rect) const {

    //get top left and bottom right in world coordinates
    //find left, right, top, and bottom edges and create the rectangle
    glm::vec2 topLeftObjectSpace = convertToObjectSpace(glm::vec2(rect.left, rect.top));
    glm::vec2 bottomRightObjectSpace = convertToObjectSpace(glm::vec2(rect.left, rect.top) + glm::vec2(rect.width, rect.height));

    float left = glm::min(topLeftObjectSpace.x, bottomRightObjectSpace.x);
    float top = glm::min(topLeftObjectSpace.y, bottomRightObjectSpace.y);

    glm::vec2 size = topLeftObjectSpace - bottomRightObjectSpace;
    size = glm::abs(size);

    sf::FloatRect boundingBox(left, top, size.x, size.y);

    return boundingBox;
}

glm::vec2 ObjectSpaceManager::convertToWorldSpace(const glm::vec2& worldSpacePosition) const {

    return objectToWorldSpace * worldSpacePosition;
}

void ObjectSpaceManager::setPositionWorldSpace(const glm::vec2& worldPosition) {

    positionInObjectSpace = worldToObjectSpace * worldPosition;
}

void ObjectSpaceManager::setPositionObjectSpace(const glm::vec2& objectSpacePosition) {

    positionInObjectSpace = objectSpacePosition;
}

void ObjectSpaceManager::move(const glm::vec2& objectSpaceDisplacement) {

    positionInObjectSpace += objectSpaceDisplacement;
}

glm::vec2 ObjectSpaceManager::getPositionWorldSpace() const {

    return objectToWorldSpace * positionInObjectSpace;
}

glm::vec2 ObjectSpaceManager::getPositionObjectSpace() const {

    return positionInObjectSpace;
}

glm::vec2 ObjectSpaceManager::getSizeObjectSpace() const {

    return sizeInObjectSpace;
}

glm::vec2 ObjectSpaceManager::getSizeWorldSpace() const {

    //to calculate size you need to calculate the position of the top left and bottom right corner, and find the difference
    glm::vec2 topLeftWorldSpace = getPositionWorldSpace();
    glm::vec2 bottomRightWorldSpace = convertToWorldSpace(positionInObjectSpace + sizeInObjectSpace);

    glm::vec2 size = topLeftWorldSpace - bottomRightWorldSpace;
    size = glm::abs(size);

    return size;
}

sf::FloatRect ObjectSpaceManager::getBoundingBoxWorldSpace() const {

    //get top left and bottom right in world coordinates
    //find left, right, top, and bottom edges and create the rectangle
    glm::vec2 topLeftWorldSpace = getPositionWorldSpace();
    glm::vec2 bottomRightWorldSpace = convertToWorldSpace(positionInObjectSpace + sizeInObjectSpace);

    float left = glm::min(topLeftWorldSpace.x, bottomRightWorldSpace.x);
    float top = glm::min(topLeftWorldSpace.y, bottomRightWorldSpace.y);
    glm::vec2 size = getSizeWorldSpace();

    sf::FloatRect boundingBox(left, top, size.x, size.y);

    return boundingBox;
}

sf::FloatRect ObjectSpaceManager::getBoundingBoxObjectSpace() const {

    sf::FloatRect boundingBox(positionInObjectSpace.x, positionInObjectSpace.y,
                              sizeInObjectSpace.x, sizeInObjectSpace.y);

    return boundingBox;
}
