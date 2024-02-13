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


#define SOLID_TILES_COUNT 2


class Entity;
class Player;
class Camera;

/**
 * @class Scene
 * @brief Classe responsable de la scene
*/
class Scene {
    public:
    Scene ();
    ~Scene ();

    void init();


    int initMap (const char *fileName);
    void unloadMap ();


    int loadEntities ();
    int loadMobEntities ();
    int loadBlockEntities ();
    int loadItemDropEntities ();

    void unloadEntities ();
    void unloadMobEntities ();
    void unloadBlockEntities ();
    void unloadItemDropEntities ();



    int getTileAt (int x, int y) const;
    std::pair<int, int> getMapDim () const;

    const std::vector<std::vector<int>> *getMap () const;
    std::list<Entity> *getMobList() const;
    std::list<Entity> *getBlockEntityList() const;
    std::list<Entity> *getItemDropList() const;
    Player *getPlayer () const;


    // void doActions (bool left, bool right, bool up, bool down, unsigned int dt);
    void update ();
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

    Player *m_player;

    int m_collisionIdArray[SOLID_TILES_COUNT];


    
    std::vector<std::vector<int>> *m_map;


    std::list<Entity> *m_mobList;
    std::list<Entity> *m_blockEntityList;
    std::list<Entity> *m_itemDropList;

    std::list<Entity*> *m_visibleEntities;

    
};







#endif