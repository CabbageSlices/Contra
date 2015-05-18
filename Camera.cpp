#include "Camera.h"
#include "glm/gtx/compatibility.hpp"

#include <iostream>
#include "GlobalConstants.h"

using std::cout;
using std::endl;
using std::vector;

Camera::Camera(sf::RenderWindow &window) :
    view(),
    defaultSize(1024, 768),
    currentSize(1024, 768),
    targetSize(1024, 768),
    currentPosition(0, 0),
    targetPosition(0, 0),
    snapThreshold(50)
    {
        setupDefaultProperties(window);
    }

void Camera::setupDefaultProperties(sf::RenderWindow &window) {

    //default view size is the size of the window
    sf::Vector2u windowSize = window.getSize();

    defaultSize = glm::vec2(windowSize.x, windowSize.y) * 1.5f;
    resetZoom();
}

void Camera::resetZoom() {

    currentSize = defaultSize;
    targetSize = defaultSize;
}

void Camera::calculateProperties(const vector<glm::vec2> &targets) {

    if(targets.size() == 0) {

        finishAllTransitions();
        return;
    }

    glm::vec2 centerPos = calculateCenterPosition(targets);
    targetPosition = centerPos;

    glm::vec2 size = calculateSize(targets);
    targetSize = size;
}

void Camera::update(const float &delta, const sf::FloatRect &worldBounds) {

    handleTransitions(delta);
    calculateView(worldBounds);
}

void Camera::applyCamera(sf::RenderWindow &window) const {

    window.setView(view);
}

sf::FloatRect Camera::getCameraBounds() const {

    sf::Vector2f size = view.getSize();
    sf::Vector2f center = view.getCenter();

    sf::Vector2f topLeft = center - size / 2.f;

    return sf::FloatRect(topLeft.x, topLeft.y, size.x, size.y);
}

void Camera::handleTransitions(const float &delta) {

    if(checkShouldSnapPosition()) {

        snapToTargetPosition();

    } else {

        glm::vec2 velocity = calculateVelocity();
        currentPosition += velocity * delta;
    }

    if(checkShouldSnapSize()) {

        snaptToTargetSize();

    } else {

        glm::vec2 sizeTransitionRate = calculateSizeTransitionRate();
        currentSize += sizeTransitionRate * delta;
    }
}

void Camera::snapToTargetPosition() {

    currentPosition = targetPosition;
}

void Camera::snaptToTargetSize() {

    currentSize = targetSize;
}

void Camera::finishAllTransitions() {

    snapToTargetPosition();
    snaptToTargetSize();
}

bool Camera::checkShouldSnapProperty(glm::vec2 currentValue, glm::vec2 targetValue) const {

    glm::vec2 distance = glm::abs(targetValue - currentValue);
    float minimumDistance = glm::min(distance.x, distance.y);

    return minimumDistance < snapThreshold;
}

bool Camera::checkShouldSnapPosition() const {

    return checkShouldSnapProperty(currentPosition, targetPosition);
}

bool Camera::checkShouldSnapSize() const {

    return checkShouldSnapProperty(currentSize, targetSize);
}

glm::vec2 Camera::calculateVelocity() const {

    glm::vec2 velocity(0, 0);

    if(currentPosition.x < targetPosition.x) {

        velocity.x = TERMINAL_VELOCITY * 2;

    } else if(currentPosition.x > targetPosition.x) {

        velocity.x = -TERMINAL_VELOCITY * 2;

    } else {

        velocity.x = 0;
    }

    if(currentPosition.y < targetPosition.y) {

        velocity.y = TERMINAL_VELOCITY * 2;

    } else if(currentPosition.y > targetPosition.y) {

        velocity.y = -TERMINAL_VELOCITY * 2;

    } else {

        velocity.y = 0;
    }

    return velocity * METERS_TO_PIXEL_RATIO;
}

glm::vec2 Camera::calculateSizeTransitionRate() const {

    glm::vec2 rate(0, 0);

    const float UNIFORM_ZOOM_RATE = 400.f;

    //determine wheter its zooming in or out
    if(currentSize.x < targetSize.x) {

        //zooming out
        rate.x = UNIFORM_ZOOM_RATE;

    } else if(currentSize.x > targetSize.x) {

        //zooming in
        rate.x = -UNIFORM_ZOOM_RATE;

    } else {

        rate.x = 0;
    }

    //you have to maintain aspect ratio so make the vertical zoom rate based on the horizontal
    float aspectRatio = defaultSize.x / defaultSize.y;

    rate.y = rate.x / aspectRatio;

    return rate;
}

glm::vec2 Camera::calculateSize(const vector<glm::vec2> &targets) {

    //determine how far apart all targets are
    //find the maximum differece on either axis
    //use the maximum difference to determine size of viewing region
    glm::vec2 minimumPosition(99999, 99999);
    glm::vec2 maximumPosition(-99999, -99999);

    for(unsigned  i = 0; i < targets.size(); ++i) {

        minimumPosition.x = glm::min(minimumPosition.x, targets[i].x);
        minimumPosition.y = glm::min(minimumPosition.y, targets[i].y);
        maximumPosition.x = glm::max(maximumPosition.x, targets[i].x);
        maximumPosition.y = glm::max(maximumPosition.y, targets[i].y);
    }

    float horizontalDifference = glm::abs(minimumPosition.x - maximumPosition.x);
    float verticalDifference = glm::abs(minimumPosition.y - maximumPosition.y);

    //the camera needs to zoom uniformly on both axis
    //so calculate on which axis you need to zoom out the furthest
    float horizontalRatio = horizontalDifference / defaultSize.x;
    float verticalRatio = verticalDifference / defaultSize.y;

    //use the larger ratio to scale the default size
    //however scale the size by a bit more than the actual ratio
    //this way the targets aren't at the edge of the camera
    float defaultSizeScale = 1.4;
    defaultSizeScale *= glm::max(horizontalRatio, verticalRatio);

    glm::vec2 size = defaultSize * defaultSizeScale;

    //make sure the size is within the bounds
    //only need to check on one axis since the aspect ratio is maintained
    if(size.x < defaultSize.x) {

        size = defaultSize;
    }

    if(size.x > calculateMaxSize().x) {

        size = calculateMaxSize();
    }

    return size;
}

glm::vec2 Camera::calculateMaxSize() {

    float maxScale = 1.5;

    return defaultSize * maxScale;
}

glm::vec2 Camera::calculateCenterPosition(const vector<glm::vec2> &targets) {

    glm::vec2 averagePosition(0, 0);

    for(unsigned i = 0; i < targets.size(); ++i) {

        averagePosition += targets[i];
    }

    averagePosition /= targets.size();
    return averagePosition;
}

void Camera::calculateView(const sf::FloatRect &worldBounds) {

    glm::vec2 centerPosition = currentPosition;

    centerPosition.x = glm::clamp(centerPosition.x, worldBounds.left + currentSize.x / 2, worldBounds.left + worldBounds.width - currentSize.x / 2);
    centerPosition.y = glm::clamp(centerPosition.y, worldBounds.top + currentSize.y / 2, worldBounds.top + worldBounds.height - currentSize.y / 2);

    view.setSize(currentSize.x, currentSize.y);
    view.setCenter(centerPosition.x, centerPosition.y);
}
