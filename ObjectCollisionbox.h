#ifndef OBJECTHITBOX_H_INCLUDED
#define OBJECTHITBOX_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "glm/glm.hpp"

#include <map>
#include <vector>

const int DEFAULT_STATE = 0;

//keeps track of an object's collisionboxes, saves multiple collisionboxes but only allows one collisionbox to be active at a given time
//all collisionboxes are positioned relative to the given origin
//the origin should be the position of the object
//and then collisionboxes can be positioned relative to the object's position
//this way you can specify collisionbox positions in object space rather than world space
class ObjectCollisionbox {

    public:

        //size in pixels
        ObjectCollisionbox();

        //given collisionbox's position should be relative to the origin of the object
        void insertCollisionbox(const sf::FloatRect &collisionboxObjectSpace, const unsigned &state = DEFAULT_STATE);

        //after removal the active collisionbox isn't guaranteed to be correct, so you must set the collisionbox again
        void removeCollisionbox(const unsigned &id, const unsigned &state = DEFAULT_STATE);

        void clearCollisionboxes();

        void scale(const float &xFactor, const float &yFactor);

        //translate the origin
        void move(const glm::vec2& displacement);

        //all in pixels
        void setOrigin(const glm::vec2& position);

        ///VERY IMPORTANT, THE HITBOX FRAME COMES FIRST, AND THEN THE ANIMATION STATE COMES SECOND
        void setActiveCollisionbox(const unsigned &id, const unsigned &state = DEFAULT_STATE); //does nothing if id is invalid

        glm::vec2 getOrigin() const;
        unsigned getCollisionboxCount(const unsigned &state = DEFAULT_STATE) const;

        //returns a collisionbox of size 0 if there are no collisionboxes
        sf::FloatRect getActiveCollisionboxWorldSpace() const;
        sf::FloatRect getCollisionboxWorldSpace(const unsigned &id, const unsigned &state = DEFAULT_STATE) const;
        sf::FloatRect getTotalCollisionboxWorldSpace(const unsigned &state = DEFAULT_STATE) const;//returns a collisionbox that encompasses all of the objects collisionboxes for the given state
        sf::FloatRect getTotalCollisionboxWorldSpace() const;//returns the encompassing collisionbox for the current state

        sf::FloatRect getActiveCollisionboxObjectSpace() const;
        sf::FloatRect getCollisionboxObjectSpace(const unsigned &id, const unsigned &state = DEFAULT_STATE) const;

    private:

        bool checkIdValid(const unsigned &id, const unsigned &state = DEFAULT_STATE) const;

        glm::vec2 origin;

        //keep track of collisionboxes, in object space
        //keep a set of collisionboxes for each state the object can be in
        //make this mutable because you can't use this through  const functions
        mutable std::map<unsigned, std::vector<sf::FloatRect> > collisionboxes;

        unsigned currentState;
        unsigned idActiveCollisionbox;
};

#endif // OBJECTSPACEMANAGER_H_INCLUDED
