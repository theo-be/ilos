#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>

#include "Vector.hpp"


class Camera {

    public:

    Camera (int x, int y, int w, int h);

    const Vector2D *getPosition () const;
    const SDL_Rect *getBoundingBox () const;
    void moveTo (int x, int y);
    void moveBy (int x, int y);

    protected:
    Vector2D m_position;
    SDL_Rect m_boundingBox;
    float m_zoom;


};





#endif