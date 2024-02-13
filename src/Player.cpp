
#include <iostream>
#include <string>
#include <list>

#include "App.hpp"
#include "Player.hpp"
#include "Entity.hpp"

using namespace std;


Player::Player() : m_target(nullptr)
{}

Player::Player (Entity *target) : m_target(target) 
{}

Player::~Player () {
    
}


void Player::doActions (list<Entity> &entityList, unsigned int dt) {

    m_target->setAvailableInteraction(false);

    
    m_target->reduceInvincibilityTime(dt);

    // move()

    // if (checkEntityCollision(entityList, *m_target)) {
    //     // fonction qui fait les interactions
    //     m_target->checkEntityInteractions();
    // }

    

}

void Player::update() {
    m_target->setAvailableInteraction(false);

    m_target->reduceInvincibilityTime(App::getDeltaTime());
}


Entity *Player::getTarget () const {
    return m_target;
}

void Player::setTarget (Entity *target) {
    m_target = target;
}
