#include "DynamicObject.h"

glm::vec2 calculateCollisionResolutionTranslation(sf::FloatRect rectA, sf::FloatRect rectB) {

    //not intersecting at all
    if(!rectA.intersects(rectB)) {

        return glm::vec2(0, 0);
    }

    //smallest overlap on x and y axis
    float distanceBRightALeft = rectB.left + rectB.width - rectA.left;
    float distanceARightBLeft = rectA.left + rectA.width - rectB.left;
    float distanceBBottomATop = rectB.top + rectB.height - rectA.top;
    float distanceABottomBTop = rectA.top + rectA.height - rectB.top;

    //find minimum displacement on x and y axis
    float minX = glm::min(distanceBRightALeft, distanceARightBLeft);
    float minY = glm::min(distanceBBottomATop, distanceABottomBTop);

    if(minX < minY) {

        //x displacement is smaller so translate on horizontal direction
        if(distanceBRightALeft < distanceARightBLeft) {

            return glm::vec2(distanceBRightALeft, 0);

        } else {

            return glm::vec2(-distanceARightBLeft, 0);
        }
    }

    if(distanceBBottomATop < distanceABottomBTop) {

        return glm::vec2(0, distanceBBottomATop);

    } else {

        return glm::vec2(0, -distanceABottomBTop);
    }

    return glm::vec2(0, 0);
}
