#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <utility>

#include "Camera.hpp"
#include "Entity.hpp"


class Entity;
class Player;
class ShowEntity;
class Camera;

/**
 * @class Scene
 * @brief Classe responsable de la scene
*/
class Scene {
    public:
    Scene ();
    Scene (SDL_Renderer *renderer);
    ~Scene ();
    int initMap (const char *fileName);
    void unloadMap ();

    int loadTiles ();
    void unloadTiles ();

    int loadEntities ();
    int loadMobEntities ();
    int loadBlockEntities ();
    int loadItemDropEntities ();

    void unloadEntities ();
    void unloadMobEntities ();
    void unloadBlockEntities ();
    void unloadItemDropEntities ();

    int loadEntityTextures ();
    int loadMobEntityTextures ();
    int loadBlockEntityTextures ();
    int loadItemDropEntityTextures ();


    void unloadEntityTextures ();
    void unloadMobEntityTextures ();
    void unloadBlockEntityTextures ();
    void unloadItemDropEntityTextures ();
    



    int getTileAt (int x, int y) const;
    std::pair<int, int> getMapDim () const;
    SDL_Rect getCameraPos () const;
    Camera *getCamera () const;
    const std::vector<std::vector<int>> *getMap () const;
    std::list<Entity> *getMobList() const;
    std::list<Entity> *getBlockEntityList() const;
    std::list<Entity> *getItemDropList() const;
    Player *getPlayer () const;
    std::vector<SDL_Texture*> *getMobEntityTextures () const;
    int getTileSize () const;

    void setTileSize (int value);
    void increaseTileSize ();
    void decreaseTileSize ();

    void displayMap () const;
    void displayScene () const;

    void setCameraPos (int x, int y);

    void doActions (bool left, bool right, bool up, bool down, unsigned int dt);
    void moveCamera (const Entity &entity);
    bool isInsideMap (float x, float y) const;
    bool isSolidTile (int blockX, int blockY) const;
    bool isOOB (int x, int y) const;
    void killEntity (Entity &e);
    void addEntityToPlayerPos ();

    void addVisibleEntity (Entity *e);
    void removeVisibleEntity (Entity *e);

    void initPlayer ();

    

    protected:
    int m_mapWidth;
    int m_mapHeight;
    SDL_Rect m_cameraRect; // inutilise
    Camera *m_camera;
    SDL_Renderer *m_renderer;
    Player *m_player;
    int m_tileSize;
    int m_collisionIdArray[SOLID_TILES_COUNT];

    int m_windowWidth;
    int m_windowHeight;

    int m_mapRenderWidth;
    int m_mapRenderHeight;

    int m_maxAreaWidth;
    int m_maxAreaHeight;

    
    std::vector<std::vector<int>> *m_map;
    std::vector<SDL_Texture*> *m_tiles;

    std::vector<SDL_Texture*> *m_mobEntityTextures;
    std::vector<SDL_Texture*> *m_blockEntityTextures;
    std::vector<SDL_Texture*> *m_itemDropEntityTextures;

    std::list<Entity> *m_mobList;
    std::list<Entity> *m_blockEntityList;
    std::list<Entity> *m_itemDropList;

    std::list<Entity*> *m_visibleEntities;


    // foncteur inutilise
    ShowEntity *m_showEntities;
    
};







#endif