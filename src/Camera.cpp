
#include <SDL2/SDL.h>


#include "Vector.hpp"
#include "Camera.hpp"


Camera::Camera (int x, int y, int w, int h) : m_position(x  + w / 2, y + h / 2),  m_boundingBox({x, y, w, h}), m_zoom(1.)
{}

const Vector2D *Camera::getPosition () const {
    return &m_position;
}

const SDL_Rect *Camera::getBoundingBox () const {
    return &m_boundingBox;
}

void Camera::moveBy (int x, int y) {
    m_position.moveBy(x, y);
    m_boundingBox.x += x;
    m_boundingBox.y += y;    
}

void Camera::moveTo (int x, int y) {
    m_position.moveTo(x, y);
    m_boundingBox.x = x - m_boundingBox.w / 2;
    m_boundingBox.y = y - m_boundingBox.h / 2;
}