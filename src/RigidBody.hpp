#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include <SDL2/SDL.h>

#include "Vector.hpp"

class RigidBody {

    public:
    RigidBody(Vector2D position, SDL_FRect hitbox) {}
    ~RigidBody() {}
    private:
    Vector2D m_position;
    Vector2D m_velocity;
    Vector2D m_acceleration;
    SDL_FRect m_hitbox;
};


#endif