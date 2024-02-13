/** 
 * @file Entity.hpp
 * @date 29/04/2023
 * @brief Classe Entity
*/

#ifndef ENTITY_H
#define ENTITY_H


#include <SDL2/SDL.h>

#include <iostream>
#include <utility>
#include <string>
#include <list>
#include <vector>

#include "Vector.hpp"
#include "Scene.hpp"

enum InteractionType {None, Talk};

class Scene;

/**
 * @class Entity
 * @brief Classe des entites
*/
class Entity {
    public:

    // constructeurs
    Entity (Scene *source);
    Entity (float x, float y, float w, float h, int hp, bool passive, const std::string &name, int textureId, int dialogueId);


    // destructeur
    ~Entity ();

    // methodes de lecture

    int getId () const;
    SDL_FRect getHitbox () const;
    int getHp () const;
    bool isPassive () const;
    int getFlightTime () const;
    bool doTouchGround () const;
    bool hasAvailableInteraction () const;
    InteractionType getinteractionType () const;
    int getTextureId () const;
    int getInvincibilityTime () const;
    Entity *getInteractionEntityTarget () const;
    bool isVisible () const;

    Vector2D getVelocity () const;
    Vector2D getVelocity0 () const;
    Vector2D getAcceleration () const;
    Vector2D getPosition () const;
    Vector2D getGlobalPosition () const;
    Vector2D getPosition0 () const;
    std::string getName () const;
    const char *getNamePtr () const;

    static int getCount ();

    static void increaseCount ();
    static void decreaseCount ();

    // methodes d'affectation

    void setHitbox(SDL_FRect &hitbox);
    void setHp (int hp);
    void setPassive (bool state);
    void setTextureId (int id);
    void setFlightTime (int time);
    void setTouchGround (bool state);
    void setAvailableInteraction (bool state);
    void setinteractionType (InteractionType id);
    void setVelocity (Vector2D &vel);
    void setVelocity0 (Vector2D &vel);
    void setAcceleration (Vector2D &acc);
    void setPosition (Vector2D &pos);
    void setPosition0 (Vector2D &pos);
    void setName (std::string &name);
    void setInvincibilityTime (int time);
    void addEntityColliding (Entity* entity);
    void clearEntityColliding ();

    // methodes d'actions

    // constantes
    
    void show (SDL_Renderer *renderer, const SDL_Rect &camera, int tileSize, std::vector<SDL_Texture*> &entityTextures) const;
    bool collideWith (Entity &entity) const;

    // non constantes

    void kill (std::list<Entity> &entityList, Entity &e);
    // void move (Scene &scene, bool left, bool right, bool up, bool down, unsigned int dt, SDL_Renderer*);
    void move (Scene &scene);
    void moveToWorld (Scene &scene, float dx, float dy, SDL_Renderer*);
    void reduceInvincibilityTime (float time);
    void checkEntityInteractions ();
    void doInteractions ();
    void checkIfVisible (SDL_Rect &camera, int tileSize);
    void calculateGlobalPosition();


    void move();


    protected:
    int m_id;
    SDL_FRect m_hitbox;
    SDL_FRect m_testHitbox;
    int m_hp;
    bool m_passive;
    int m_textureId;
    bool m_touchGround;
    int m_flightTime;
    int m_invincibilityTime;
    bool m_availableInteraction;
    InteractionType m_interactionType;
    Entity *m_interactionTarget;
    bool m_playable;
    bool m_visible;

    std::string m_name;

    std::list<Entity*> m_collideWith;

    std::list<Entity*> m_children;
    Entity *m_parent;

    Scene *m_scene;


    // position

    Vector2D m_position;
    Vector2D m_position0;
    Vector2D m_velocity0;
    Vector2D m_velocity;
    Vector2D m_acceleration;
    Vector2D m_tilePosition;

    Vector2D m_globalPosition;

    static int m_count;
    static int m_nextAvailableId;




    private:



};


bool operator== (Entity const &a, Entity const &b);
bool operator!= (Entity const &a, Entity const &b);




#endif