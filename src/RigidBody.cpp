
#include <SDL2/SDL.h>

#include "RigidBody.hpp"
#include "Vector.hpp"


RigidBody::RigidBody (Vector2D position, SDL_FRect hitbox) : m_position(position), m_hitbox(hitbox)
{}


RigidBody::~RigidBody () {

}


