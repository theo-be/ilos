#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>
#include <vector>
#include <list>

#include "Vector.hpp"

#include "Entity.hpp"

class Entity;

class Camera {

    public:

    // Camera (float x, float y, float w, float h);
    Camera (float x, float y, float sceneWidth, float sceneHeight, float windowWidth, float windowHeight, SDL_Renderer *renderer);

    const Vector2D getPosition () const;
    const SDL_FRect getBoundingBox () const;
    float getTileSize () const;

    // temporaire
    void draw(SDL_Renderer *r, std::vector<SDL_Texture*> *bTextures, std::vector<std::vector<int>> *map);
    void drawE(SDL_Renderer *r, std::list<Entity> *list, std::vector<SDL_Texture*> *texture);

    void moveTo (float x, float y);
    void moveBy (float x, float y);
    void moveBy (const Vector2D &vector);

    void snapToMap ();

    void centerToTarget ();

    void lockTo (Entity &entity);
    void lock ();
    void lockX ();
    void lockY ();

    void unlock ();
    void unlockX ();
    void unlockY ();


    void setMode (CameraMode mode);

    void displayMap (const std::vector<std::vector<int>> *map);
    void displayEntities (const std::list<Entity> *entityList);
    void update ();

    void setWindowDimension (int x, int y);
    void setSceneDimension (float x, float y);


    static void loadTilesTextures (const char *fileName);
    static void unloadTilesTextures ();

    static void loadEntityTextures (const char *fileName);
    static void unloadEntityTextures ();

    protected:

    // fonction qui verifie qu'on sort pas de la carte

    void move (); // ??

    Vector2D m_position; // en tuiles

    float m_sceneWidth;
    float m_sceneHeight;

    float m_screenWidth;
    float m_screenHeight;


    // SDL_FRect m_boundingBox;
    float m_zoom;
    CameraMode m_cameraMode;
    float m_tileSize;

    int m_windowWidth;
    int m_windowHeight;

    bool m_lockedX;
    bool m_lockedY;

    // float m_targetScreenRatioX;
    // float m_targetScreenRatioY;

    Entity *m_target;

    static SDL_Renderer *m_renderer;

    static std::vector<SDL_Texture*> *m_mobEntityTextures;
    static std::vector<SDL_Texture*> *m_tiles;

};





#endif