
#include <iostream>
#include <string>
#include <list>

#include "Player.hpp"
#include "Entity.hpp"

#include "entities.hpp"

using namespace std;


Player::Player() : m_target(nullptr)
{}

Player::Player (Entity *target) : m_target(target) 
{}

// Player::~Player () {
    
// }


void Player::doActions (list<Entity> &entityList, unsigned int dt) {

    m_target->setAvailableInteraction(false);

    
    m_target->reduceInvincibilityTime(dt);

    // move()

    if (checkEntityCollision(entityList, *m_target)) {
        // fonction qui fait les interactions
        m_target->checkEntityInteractions();
    }

    

}


Entity *Player::getTarget () const {
    return m_target;
}

void Player::setTarget (Entity *target) {
    m_target = target;
}

/*

player->available_interaction = 0;

// regarder si on touche l'entite

Entity *collide_with = checkEntityCollision(entity_list, player);

if (collide_with != NULL) {
    if (collide_with->passive) {
        player->available_interaction = 1;

        // displayFont(renderer, font_texture, "E", collide_with->hitbox.x, collide_with->hitbox.h - collide_with->hitbox.h * 2 - 10, TILE_SIZE);
    } else {
        // Degats infilges
        if (player->invincibility_time <= 0.0) {
            player->hp--;
            player->invincibility_time = 2.0;
            printf("Degat inflige par %s, %d points de vie restants\n", collide_with->name, player->hp);
        }
    
    }
}

*/