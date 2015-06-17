#ifndef OBJECTHITBOX_H_INCLUDED
#define OBJECTHITBOX_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "glm/glm.hpp"

#include <map>
#include <vector>

const int DEFAULT_STATE = 0;

//keeps track of an object's hitboxes, saves multiple hitboxes but only allows one hitbox to be active at a given time
//all hitboxes are positioned relative to the given origin
//the origin should be the position of the object
//and then hitboxes can be positioned relative to the object's position
//this way you can specify hitbox positions in object space rather than world space
class ObjectHitbox {

    public:

        //size in pixels
        ObjectHitbox();

        //given hitbox's position should be relative to the origin of the object
        void insertHitbox(const sf::FloatRect &hitboxObjectSpace, const unsigned &state = DEFAULT_STATE);

        //after removal the active hitbox isn't guaranteed to be correct, so you must set the hitbox again
        void removeHitbox(const unsigned &id, const unsigned &state = DEFAULT_STATE);

        void clearHitboxes();

        //translate the origin
        void move(const glm::vec2& displacement);

        //all in pixels
        void setOrigin(const glm::vec2& position);
        void setActiveHitbox(const unsigned &id, const unsigned &state = DEFAULT_STATE); //does nothing if id is invalid

        glm::vec2 getOrigin() const;
        unsigned getHitboxCount(const unsigned &state = DEFAULT_STATE) const;

        //returns a hitbox of size 0 if there are no hitboxes
        sf::FloatRect getActiveHitboxWorldSpace() const;
        sf::FloatRect getHitboxWorldSpace(const unsigned &id, const unsigned &state = DEFAULT_STATE) const;
        sf::FloatRect getTotalHitboxWorldSpace(const unsigned &state = DEFAULT_STATE) const;//returns a hitbox that encompasses all of the objects hitboxes for the given state
        sf::FloatRect getTotalHitboxWorldSpace() const;//returns the encompassing hitbox for the current state

        sf::FloatRect getActiveHitboxObjectSpace() const;
        sf::FloatRect getHitboxObjectSpace(const unsigned &id, const unsigned &state = DEFAULT_STATE) const;

    private:

        bool checkIdValid(const unsigned &id, const unsigned &state = DEFAULT_STATE) const;

        glm::vec2 origin;

        //keep track of hitboxes, in object space
        //keep a set of hitboxes for each state the object can be in
        //make this mutable because you can't use this through  const functions
        mutable std::map<unsigned, std::vector<sf::FloatRect> > hitboxes;

        unsigned currentState;
        unsigned idActiveHitbox;
};

#endif // OBJECTSPACEMANAGER_H_INCLUDED
