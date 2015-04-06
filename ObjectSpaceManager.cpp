#include "ObjectSpaceManager.h"

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

    glm::vec2 posObjectSpace = convertToObjectSpace(glm::vec2(rect.left, rect.top));
    glm::vec2 sizeObjectSpace = convertToObjectSpace(glm::vec2(rect.width, rect.height));

    return sf::FloatRect(posObjectSpace.x, posObjectSpace.y, sizeObjectSpace.x, sizeObjectSpace.y);
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

    return objectToWorldSpace * sizeInObjectSpace;
}

sf::FloatRect ObjectSpaceManager::getBoundingBoxWorldSpace() const {

    //when converting from object space to world space you might end up with negative values for the size
    //the position of the object might not refer to the top left of the bounding box
    //so make sure that the bounding box top and left always refer to the top left of the object
    glm::vec2 worldPosition = getPositionWorldSpace();
    glm::vec2 worldSize = getSizeWorldSpace();

    sf::FloatRect boundingBox(worldPosition.x, worldPosition.y, worldSize.x, worldSize.y);

    if(worldSize.x < 0) {

        boundingBox.left += worldSize.x;
        boundingBox.width *= -1;
    }

    if(worldSize.y < 0) {

        boundingBox.top += worldSize.y;
        boundingBox.height *= -1;
    }

    return boundingBox;
}

sf::FloatRect ObjectSpaceManager::getBoundingBoxObjectSpace() const {

    sf::FloatRect boundingBox(positionInObjectSpace.x, positionInObjectSpace.y,
                              sizeInObjectSpace.x, sizeInObjectSpace.y);

    return boundingBox;
}
