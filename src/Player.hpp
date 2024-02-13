#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include <list>

#include "Entity.hpp"

/**
 * @class Player
 * @brief Classe du joueur
*/
class Player {
    public:
    Player ();
    Player (Entity *target);

    ~Player();

    void doActions (std::list<Entity> &entityList, unsigned int dt);

    void update ();

    Entity *getTarget () const;
    void setTarget (Entity *);

    private:

    Entity *m_target;

};











#endif