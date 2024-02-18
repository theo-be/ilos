#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>
#include <vector>
#include <list>

#include "Vector.hpp"

#include "Entity.hpp"

// zone pour la camera
#define CAMERA_DEFAULT_AREA_WIDTH (3. / 4.)
#define CAMERA_DEFAULT_AREA_HEIGHT (2. / 3.)


#define DEFAULT_SCENE_WIDTH 40
#define DEFAULT_SCENE_HEIGHT 22.5

#define DEFAULT_TILE_LOCATION "resources/fg"
#define DEFAULT_ENTITY_LOCATION "resources/entity"


#define TILES_TEXTURE_COUNT 5
#define ENTITY_TEXTURE_COUNT 4

enum CameraMode {Default, Free, Locked, TargetEntity, LockEntity};


class Entity;



/**
 * @class Camera
 * @brief Classe responsable de l'affichage de la scene sur l'ecran
*/
class Camera {

    public:

    Camera();
    // Camera (float x, float y, float w, float h);
    Camera (float sceneWidth, float sceneHeight, float windowWidth, float windowHeight, SDL_Renderer *renderer);
    ~Camera ();


    const Vector2D getPosition () const;
    const SDL_FRect getBoundingBox () const;
    // float getTileSize () const;

    void moveTo (float x, float y);
    void moveBy (float x, float y);
    void moveBy (const Vector2D &vector);

    void snapToMap ();

    void centerToTarget ();
    void followTarget ();
    void setTarget(Entity *e);
    void setTargetScreenArea (float ratioX, float ratioY);

    void lockTo (Entity &entity);
    void lock ();
    void lockX ();
    void lockY ();

    void unlock ();
    void unlockX ();
    void unlockY ();


    void setMode (CameraMode mode);
    void setRenderer (SDL_Renderer *renderer);

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

    // Aire sur l'ecran par rapport a la camera ou l'entite cible peut se deplacer sans que la camera ne se deplace
    SDL_FRect m_targetScreenArea;

    static SDL_Renderer *m_renderer;

    static std::vector<SDL_Texture*> *m_mobEntityTextures;
    static std::vector<SDL_Texture*> *m_tiles;

};





#endif