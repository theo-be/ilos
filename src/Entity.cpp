/** 
 * @file Entity.cpp
 * @date 29/04/2023
 * @brief Classe Entity
*/

#include <SDL2/SDL.h>

#include <iostream>
#include <string>
#include <utility>
#include <list>
#include <vector>
#include <cmath>

#include "constants.hpp"

#include "Scene.hpp"
#include "Entity.hpp"
#include "Vector.hpp"

using namespace std;

int Entity::m_count = 0;
int Entity::m_nextAvailableId = 1;

// Constructeur par defaut
Entity::Entity ()
: m_id(m_nextAvailableId), m_hp(-1), m_passive(true), m_textureId(-1), m_touchGround(false), m_flightTime(0), m_invincibilityTime(0), m_availableInteraction(false), m_interactionType(None), m_interactionTarget(nullptr), m_playable(true), m_visible(true), m_name(""), m_parent(nullptr), m_position0(0., 0.), m_velocity0(0, 100.), m_velocity(0., 0.), m_acceleration(0., 1000.)
{
    // m_count++;
    cout << "Entite cree, nombre : " << m_count << endl;
    m_nextAvailableId++;
    m_hitbox = {5, 27, 1, 1};
    m_position.setCoords(5.5, 27.5);
    m_tilePosition.setCoords(5, 27);
    m_collideWith.clear();
    m_children.clear();
}

Entity::Entity (float x, float y, float w, float h, int hp, bool passive, const string &name, int textureId, int dialogueId) 
: m_id(m_nextAvailableId), m_hp(hp), m_passive(passive), m_textureId(textureId), m_touchGround(false), m_flightTime(0), m_invincibilityTime(0), m_availableInteraction(false), m_interactionType(None), m_interactionTarget(nullptr), m_playable(true), m_visible(true), m_name(name), m_parent(nullptr), m_position0(0., 0.), m_velocity0(0., 0.), m_velocity(0., 0.), m_acceleration(0., 0.)
{
    // m_count++;
    cout << "Entite cree, nombre : " << m_count << endl;
    m_nextAvailableId++;
    m_position.setCoords(x, y);
    m_tilePosition.setCoords(x / TILE_SIZE, y / TILE_SIZE);
    m_hitbox = {x - w / 2, y - h / 2, w, h};
    m_collideWith.clear();
    m_children.clear();
}




// Destructeur
Entity::~Entity () {
    m_count--;
    m_collideWith.clear();
    m_name.clear();
    cout << "Destruction d\'entite, nombre : " << m_count << endl;
}


// methodes de lecture

int Entity::getId () const {
    return m_id;
}

SDL_FRect Entity::getHitbox () const {
    return m_hitbox;
}

int Entity::getHp () const {
    return m_hp;
}

bool Entity::isPassive () const {
    return m_passive;
}

int Entity::getTextureId () const {
    return m_textureId;
}

bool Entity::doTouchGround () const {
    return m_touchGround;
}

int Entity::getFlightTime () const {
    return m_flightTime;
}

int Entity::getInvincibilityTime () const {
    return m_invincibilityTime;
}

bool Entity::hasAvailableInteraction () const {
    return m_availableInteraction;
}

InteractionType Entity::getinteractionType () const {
    return m_interactionType;
}

string Entity::getName () const {
    return m_name;
}

const char *Entity::getNamePtr () const {
    return m_name.c_str();
}

Vector2D Entity::getPosition () const {
    return m_position;
}

Vector2D Entity::getPosition0 () const {
    return m_position0;
}

Vector2D Entity::getGlobalPosition () const {
    return m_globalPosition;
}

Vector2D Entity::getVelocity0 () const {
    return m_velocity0;
}

Vector2D Entity::getVelocity () const {
    return m_velocity;
}

Vector2D Entity::getAcceleration () const {
    return m_acceleration;
}

Entity *Entity::getInteractionEntityTarget () const {
    return m_interactionTarget;
}

bool Entity::isVisible () const {
    return m_visible;
}

// methodes d'affectation


void Entity::setHitbox (SDL_FRect &hitbox) {
    m_hitbox = hitbox;
}

void Entity::setHp (int hp) {
    m_hp = hp;
}

void Entity::setPassive (bool state) {
    m_passive = state;
}

void Entity::setTextureId (int id) {
    m_textureId = id;
}

void Entity::setTouchGround (bool state) {
    m_touchGround = state;
}

void Entity::setFlightTime (int time) {
    m_flightTime = time;
}

void Entity::setInvincibilityTime (int time) {
    m_invincibilityTime = time;
}

void Entity::setAvailableInteraction (bool state) {
    m_availableInteraction = state;
}

void Entity::setinteractionType (InteractionType id) {
    m_interactionType = id;
}

void Entity::setName (string &name) {
    m_name = name;
}

void Entity::setPosition (Vector2D &pos) {
    m_position = pos;
}

void Entity::setPosition0 (Vector2D &pos) {
    m_position0 = pos;
}

void Entity::setVelocity0 (Vector2D &vel) {
    m_velocity0 = vel;
}

void Entity::setVelocity (Vector2D &vel) {
    m_velocity = vel;
}

void Entity::setAcceleration (Vector2D &acc) {
    m_acceleration = acc;
}

void Entity::addEntityColliding (Entity *entity) {
    m_collideWith.push_front(entity);
}

void Entity::clearEntityColliding () {
    m_collideWith.clear();
}



// Methode statique
int Entity::getCount () {
    return m_count;
}

void Entity::increaseCount () {
    m_count++;
}
void Entity::decreaseCount () {
    m_count--;
}

// autres methodes

/**
 * @fn void Entity::kill (list<Entity> &entityList, Entity &e)
 * @param entityList liste des entites
 * @param e entite a tuer
 * @brief tue une entite, pour le moment, la retire de la liste des entites
*/
void Entity::kill (list<Entity> &entityList, Entity &e) {
    if ((*this) != e) {
        entityList.remove(e);
        cout << m_name << " a tue " << e.m_name;
    }
}


/**
 * @fn void Entity::show (SDL_Renderer *renderer, Scene &scene, vector<SDL_Texture*> &entityTextures) const
 * @param renderer renderer
 * @param scene scene
 * @param entityTextures liste des textures
 * @brief affiche l'entite
*/
void Entity::show (SDL_Renderer *renderer, const SDL_Rect &camera, int tileSize, vector<SDL_Texture*> &entityTextures) const {
    SDL_FRect screenPosition = {
        m_hitbox.x * tileSize - camera.x,
        m_hitbox.y * tileSize - camera.y,
        m_hitbox.w * tileSize, m_hitbox.h * tileSize
    };
    if (m_textureId > -1)
        SDL_RenderCopyF(renderer, entityTextures.at(m_textureId), NULL, &screenPosition);
}

/**
 * @fn void Entity::move (Scene &scene, bool left, bool right, bool up, bool down, unsigned int dt, SDL_Renderer *r)
 * @param scene scene
 * @param left mouvement a gauche
 * @param right mouvement a droite
 * @param up mouvement en haut
 * @param down mouvement en bas
 * @param dt temps en ms
 * @param r renderer
 * @brief deplace une entite
 * @see Entity::moveToWorld
*/
/*void Entity::move (Scene &scene, bool left, bool right, bool up, bool down, unsigned int dt, SDL_Renderer *r) {
    // Si l'entite touche le sol
    if (m_touchGround) {
        m_position0.first = m_hitbox.x;
        m_position0.second = m_hitbox.y;
        m_velocity0.second = 0.0;
        m_flightTime = 0.0;
    }
    // Si l'entite est jouable
    if (m_playable) {
        // Deplacements lateraux
        if ((left || right) && !(left && right)) {
            m_velocity.first = (left ? -(m_velocity0.first) : m_velocity0.first);
            m_velocity.first *= dt / 1000.0;
            moveToWorld(scene, m_velocity.first, 0.0, r);
        }


        
        // Saut
        if (up && m_touchGround) {
            // jumping = 1;
            m_velocity0.second = -600.0;
            m_flightTime = 0;
        }
    }
    // position en tuiles
    // int entityMapX = m_hitbox.x / TILE_SIZE;
    // int entityMapY = m_hitbox.y / TILE_SIZE;
    m_touchGround = false;
    float t = m_flightTime / 1000.0;
    m_flightTime += dt;
    float newY = m_acceleration.second * t * t / 2.0 + m_velocity0.second * t + m_position0.second;
    // Difference de position entre la precedente et la prochaine
    float dy = newY - (float)m_hitbox.y;
    m_velocity.second = dy;
    moveToWorld(scene, 0.0, dy, r);


    pair<int, int> dim = scene.getMapDim();
    // Eviter de sortir de la carte
    m_hitbox.x = min<int>(max<int>(m_hitbox.x, 0), dim.first * TILE_SIZE - m_hitbox.w);
    m_hitbox.y = min<int>(max<int>(m_hitbox.y, 0), dim.second * (TILE_SIZE - 1) - m_hitbox.h);
}*/

/**
 * @fn void Entity::moveToWorld (Scene &scene, float dx, float dy, SDL_Renderer *r)
 * @param scene scene
 * @param dx decalage en x de la position
 * @param dy decalage en y de la position
 * @param r renderer
 * @brief deplace une entite sur la carte
 * @see Entity::move
*/
/*void Entity::moveToWorld (Scene &scene, float dx, float dy, SDL_Renderer *r) {
    // Position en tuiles
    int entityMapX, entityMapY, hit, adj;
    // debug
    SDL_Rect re = {0, 0, TILE_SIZE, TILE_SIZE};
    SDL_Point cameraPos = scene.getCameraPos();
    // Deplacement horizontal
    if (dx) {
        SDL_SetRenderDrawColor(r, 255, 0, 0, 200); // Debug
        m_hitbox.x += dx;
        // point superieur du cote vers lequel avance le joueur
        entityMapX = (dx > 0 ? m_hitbox.x + m_hitbox.w : m_hitbox.x) / TILE_SIZE;

        entityMapY = m_hitbox.y / TILE_SIZE;
        re.x = (entityMapX * TILE_SIZE - cameraPos.x);
        re.y = (entityMapY * TILE_SIZE - cameraPos.y);
        SDL_RenderFillRect(r, &re); // Debug
        hit = 0;
        if (scene.isInsideMap(entityMapX, entityMapY)) 
            hit = true;
        // On regarde le bloc du cote inferieur pour ne pas rentrer sur le bloc en dessous
        // par ex sauter vers la droite vers un bloc en dessous
        entityMapY = (m_hitbox.y + m_hitbox.h - 1) / TILE_SIZE;
        re.x = (entityMapX * TILE_SIZE - cameraPos.x);
        re.y = (entityMapY * TILE_SIZE - cameraPos.y);
        SDL_RenderFillRect(r, &re); // Debug
        if (scene.isInsideMap(entityMapX, entityMapY))
            hit = true;
        if (hit) {
            adj = dx > 0 ? -m_hitbox.w : TILE_SIZE;
            m_hitbox.x = (entityMapX * TILE_SIZE) + adj;
        }
    }
    // Deplacement vertical
    if (dy) {
        SDL_SetRenderDrawColor(r, 0, 255, 0, 200);
        m_hitbox.y += dy;
        entityMapY = (dy > 0 ? m_hitbox.y + m_hitbox.h : m_hitbox.y) / TILE_SIZE;
        // entityMapY = (m_hitbox.y + m_hitbox.h) / TILE_SIZE; // uniquement le bloc du dessous
        entityMapX = m_hitbox.x / TILE_SIZE;
        hit = 0;
        re.x = (entityMapX * TILE_SIZE - cameraPos.x);
        re.y = (entityMapY * TILE_SIZE - cameraPos.y);
        SDL_RenderFillRect(r, &re); // Debug
        if (scene.isInsideMap(entityMapX, entityMapY)) 
            hit = true;
        entityMapX = (m_hitbox.x + m_hitbox.w - 1) / TILE_SIZE;
        re.x = (entityMapX * TILE_SIZE - cameraPos.x);
        re.y = (entityMapY * TILE_SIZE - cameraPos.y);
        SDL_RenderFillRect(r, &re); // Debug
        if (scene.isInsideMap(entityMapX, entityMapY)) 
            hit = true;
        if (hit) {
            adj = dy > 0 ? -m_hitbox.h : TILE_SIZE;
            m_hitbox.y = entityMapY * TILE_SIZE + adj;
            m_velocity0.second = dy > 0 ? 0.0 : m_velocity0.second;
            m_touchGround = dy > 0;
            m_flightTime = dy > 0 ? 0.0 : m_flightTime;
        }
    }
}*/

/**
 * @fn void Entity::reduceInvincibilityTime (float time)
 * @param time temps en ms
 * @brief Reduction du temps d'invincibilite
*/
void Entity::reduceInvincibilityTime (float time) {
    if (m_invincibilityTime > 0.0) {
        m_invincibilityTime -= time;
        if (m_invincibilityTime < 0.0) m_invincibilityTime = 0.0;
    }
}


/**
 * @fn bool Entity::collideWith (Entity &entity) const
 * @param entity entite cible
 * @returns vrai si collision, false sinon
 * @brief evalue l'etat de collision entre deux entites
*/
bool Entity::collideWith (Entity &entity) const {
    // SDL_FRect h = entity.m_hitbox;
    // return entity != *this && SDL_HasIntersection(&m_hitbox, &h);
    if (entity == *this) return false;
    if (
        (
        (entity.m_hitbox.x >= m_hitbox.x && entity.m_hitbox.x <= m_hitbox.x + m_hitbox.w)
        || (entity.m_hitbox.x + entity.m_hitbox.w >= m_hitbox.x && entity.m_hitbox.x + entity.m_hitbox.w <= m_hitbox.x + m_hitbox.w)
        )
        &&
        ((
        entity.m_hitbox.y >= m_hitbox.y && entity.m_hitbox.y <= m_hitbox.y + m_hitbox.h)
        || (entity.m_hitbox.y + entity.m_hitbox.h >= m_hitbox.y && entity.m_hitbox.y + entity.m_hitbox.h <= m_hitbox.y + m_hitbox.h)
        )
    ) 
        return true;
    return false;
}

/**
 * @fn void Entity::checkEntityInteractions ()
 * @brief fait les actions necessaires pour la collision avec chaque entite
*/
void Entity::checkEntityInteractions () {
    m_interactionTarget = nullptr;
    m_availableInteraction = false;
    m_interactionType = None;
    for (std::list<Entity*>::iterator it = m_collideWith.begin(); it != m_collideWith.end(); it++) {
        if ((*it)->m_passive) {
            m_availableInteraction = true;
            m_interactionType = Talk;
            m_interactionTarget = *it;

        } else {
            if (m_invincibilityTime <= 0 && m_passive != (*it)->m_passive) {
                m_hp--;
                m_invincibilityTime = 1000;
                cout << "Degats infliges par " << (*it)->m_name << endl;
            }
        }
    }
}

/**
 * @fn void Entity::doInteractions ()
 * @brief Effectue l'interaction sur l'entite
*/
void Entity::doInteractions () {
    if (m_availableInteraction)
    cout << "Action sur " << m_interactionTarget->getNamePtr() << endl;
    else cout << "Pas d\'action" << endl;
}



// Reecriture de la fonction move
/**
 * @fn void Entity::move (Scene &scene, bool left, bool right, bool up, bool down, unsigned int dt, SDL_Renderer *r)
 * @param scene scene
 * @param left mouvement a gauche
 * @param right mouvement a droite
 * @param up mouvement en haut
 * @param down mouvement en bas
 * @param dt temps en ms
 * @param r renderer
 * @brief deplace une entite
 * @see Entity::moveToWorld
*/
void Entity::move (Scene &scene, bool left, bool right, bool up, bool down, unsigned int dt, SDL_Renderer *r) {
    int tileSize = scene.getTileSize();
    SDL_Rect camera = scene.getCameraPos();
    pair<float, float> v = m_velocity.getCoords();
    pair<float, float> a = m_acceleration.getCoords();
    pair<float, float> p = m_position.getCoords();
    // decalage de position
    float dx = 0., dy = 0.;
    m_acceleration.setCoords(a.first, .00003125);

    if (m_playable) {

        // Horizontal
        if ((left || right) && !(left && right)) {
            v.first = (right ? 0.0125 : -0.0125);
        } else v.first = .0;
        dx = v.first * dt;
        
        // Vertical
        if (m_touchGround) {
            v.second = 0.0;
            m_flightTime = 0;
            if (up) {
                v.second = -0.01875;
                dy = v.second * dt;
                m_touchGround = false;
            }
        } else {
            m_flightTime += dt;
            v.second += a.second * dt / 2;
            dy = v.second * dt;
            v.second += a.second * dt / 2;
        }

        m_velocity.setCoords(v.first, v.second);

        moveToWorld(scene, dx, dy, r);
    }

    // Eviter de sortir de la carte
    pair<int, int> dim = scene.getMapDim();
    m_hitbox.x = min<float>(max<float>(m_hitbox.x, 0), dim.first - m_hitbox.w);
    m_hitbox.y = min<float>(max<float>(m_hitbox.y, 0), dim.second - m_hitbox.h);

    p.first = min<float>(max<float>(m_hitbox.x + m_hitbox.w / 2, 0), dim.first - (m_hitbox.w / 2));
    p.second = min<float>(max<float>(m_hitbox.y + m_hitbox.h / 2, 0), dim.second - (m_hitbox.h / 2));
    m_position.setCoords(p.first, p.second);

    // l'entite est-elle visible
    checkIfVisible(camera, tileSize);
}


// Deuxieme reecriture de la fonction moveToWorld
/*void Entity::moveToWorld (Scene &scene, int dx, int dy, SDL_Renderer *r) {

    // Position en tuile du coin superieur gauche

    int tileSize = scene.getTileSize();

    pair<float, float> v = m_velocity.getCoords();
    pair<float, float> a = m_acceleration.getCoords();

    int entityMapX = m_hitbox.x / tileSize;
    int entityMapY = m_hitbox.y / tileSize;

    SDL_Point importantCorner;
    // SDL_Point otherCorner;

    // Debug
    // SDL_Rect re = {0, 0, tileSize, tileSize};
    // SDL_Point cameraPos = scene.getCameraPos();
    
    importantCorner.x = v.first < .0 ? m_hitbox.x : m_hitbox.x + m_hitbox.w - 1;
    importantCorner.y = v.second < .0 ? m_hitbox.y : m_hitbox.y + m_hitbox.h -;

    // bool hit = 0;

    int replacementPosition = 0;

    if (dx) {
        // SDL_SetRenderDrawColor(r, 255, 0, 0, 200);
        m_hitbox.x += dx;

        // replacement horizontal

        if (scene.isInsideMap(importantCorner.x, m_hitbox.y)) {
            replacementPosition = v.first < .0 ? (entityMapX + 1) * tileSize : (entityMapX + 1) * tileSize - m_hitbox.w;
            m_hitbox.x = replacementPosition;
        }
        // re.x = importantCorner.x - cameraPos.x;
        // re.y = m_hitbox.y - cameraPos.y;
        // SDL_RenderFillRect(r, &re);

        // On regarde le bloc du cote inferieur pour ne pas rentrer sur le bloc en dessous
        // par ex sauter vers la droite vers un bloc en dessous
        else if (scene.isInsideMap(importantCorner.x, m_hitbox.y + m_hitbox.h - 1)) {
            replacementPosition = v.first < .0 ? (entityMapX + 1) * tileSize : (entityMapX + 1) * tileSize - m_hitbox.w;
            m_hitbox.x = replacementPosition;
        }
        // re.y = m_hitbox.y + m_hitbox.h - 1 - cameraPos.y;
        // SDL_RenderFillRect(r, &re);
    }

    // SDL_SetRenderDrawColor(r, 0, 255, 0, 200);
    m_hitbox.y += dy;

    // replacement vertical
    if (scene.isInsideMap(m_hitbox.x, importantCorner.y)) {
        // replacementPosition = m_velocity.second < .0 ? entityMapY + 1 : entityMapY;
        if (v.second < .0) {
            replacementPosition = (entityMapY + 1) * tileSize;
            v.second = .0005;
        } else {
            replacementPosition = (entityMapY + 1) * tileSize - m_hitbox.h;
        }
        m_hitbox.y = replacementPosition;
    }

    else if (scene.isInsideMap(m_hitbox.x + m_hitbox.w - 1, importantCorner.y)) {
        // replacementPosition = m_velocity.second < .0 ? entityMapY + 1 : entityMapY;
        if (v.second < .0) {
            replacementPosition = (entityMapY + 1) * tileSize;
            v.second = .0005;
        } else {
            replacementPosition = (entityMapY + 1) * tileSize - m_hitbox.h;
        }
        m_hitbox.y = replacementPosition;
    }

    m_velocity.setCoords(v.first, v.second);
    m_acceleration.setCoords(a.first, a.second);

    // touche le sol une fois replace ?
    if (scene.isInsideMap(m_hitbox.x, m_hitbox.y + m_hitbox.h) 
    || scene.isInsideMap(m_hitbox.x + m_hitbox.w - 1, m_hitbox.y + m_hitbox.h)) {
        m_touchGround = true;
        m_flightTime = 0;
    } else m_touchGround = false;


}*/

// Troisieme reecriture de la methode moveToWorld
// A refaire
/**
 * @fn void Entity::moveToWorld (Scene &scene, float dx, float dy, SDL_Renderer *r)
 * @param scene scene
 * @param dx decalage en x de la position
 * @param dy decalage en y de la position
 * @param r renderer
 * @brief deplace une entite sur la carte
 * @see Entity::move
*/
void Entity::moveToWorld (Scene &scene, float dx, float dy, SDL_Renderer *r) {

    int tileSize = scene.getTileSize();
    pair<float, float> p = m_position.getCoords();
    // pair<float, float> v = m_velocity.getCoords();
    // pair<float, float> a = m_acceleration.getCoords();

    SDL_Rect camera = scene.getCameraPos();

    SDL_FRect movementArea = {0};
    SDL_FRect debugRect = {0};

    bool hit;
    float replacementPosition = 0;

    // nombre de tuiles a verifier dans chaque direction
    int blockX, blockY;

    // sens de verification
    int increment;

    // a quelle tuile dans la carte commencer la verification
    int startX, startY;


    if (dx) {

        // Aire de deplacement
        if (dx < 0) {
            movementArea = {m_hitbox.x + dx, m_hitbox.y, -dx, m_hitbox.h};
        } else {
            movementArea = {m_hitbox.x + m_hitbox.w, m_hitbox.y, dx, m_hitbox.h};
        }

        SDL_SetRenderDrawColor(r, 255, 0, 0, SDL_ALPHA_OPAQUE);
        debugRect = {movementArea.x * tileSize - camera.x, movementArea.y * tileSize - camera.y, movementArea.w * tileSize, movementArea.h * tileSize};
        SDL_RenderFillRectF(r, &debugRect);

        // Verification des collisions

        // doit etre 1 pour X et taille en tuile + 1 pour Y mais peut etre est superieur avec un grand dt
        blockX = (int)movementArea.w + 1;
        blockY = (int)movementArea.h + 1;
        // si l'entite est alignee sur une tuile, il ne faut pas s'etendre une tuile de plus vers la droite
        blockY += floor(m_hitbox.y) == m_hitbox.y ? -1 : 0;

        hit = 0;
        increment = dx > 0 ? 1 : -1;

        startX = (dx > 0 ? (int)(m_hitbox.x + m_hitbox.w) : (int)(m_hitbox.x));
        startY = (int)m_hitbox.y;

        // cout << "blocXY " << blockX << ',' << blockY;
        // cout << "; startXY " << startX << ',' << startY;
        // cout << '\n';

        // verification verticale puis horizontale
        for (int i = 0; i < blockY; i++) {
            for (int j = 0; j < blockX; j++) {
                // verifier si il y a collision avec un block
                if (scene.isSolidTile(startX + j * increment, startY + i)) {
                    // cout << "toucheX" << '\n';
                    hit = 1;

                    // limiter les prochaines verifications a cette distance en tuiles
                    blockX = j;

                    // replacement
                    replacementPosition = (dx > 0 ? startX + j - m_hitbox.w : startX + j * increment + 1);
                    m_hitbox.x = replacementPosition;
                    m_position.moveTo(m_hitbox.x + m_hitbox.w / 2., p.second);
                }
            }
        }

        // si pas de collision avec une tuile
        if (!hit) {
            m_hitbox.x += dx;
            m_position.moveBy(dx, .0);
        }
    }

    if (dy) {
        if (dy < 0) {
            movementArea = {m_hitbox.x, m_hitbox.y + dy, m_hitbox.w, -dy};
        } else {
            movementArea = {m_hitbox.x, m_hitbox.y + m_hitbox.h, m_hitbox.w, dy};
        }

        SDL_SetRenderDrawColor(r, 0, 155, 0, SDL_ALPHA_OPAQUE);
        debugRect = {movementArea.x * tileSize - camera.x, movementArea.y * tileSize - camera.y, movementArea.w * tileSize, movementArea.h * tileSize};
        SDL_RenderFillRectF(r, &debugRect);

        hit = 0;
        increment = dy > 0 ? 1 : -1;

        blockX = (int)movementArea.w + 1;
        blockY = (int)movementArea.h + 1;

        blockX += floor(m_hitbox.x) == m_hitbox.x ? -1 : 0;

        startX = (int)m_hitbox.x;
        startY = dy > 0 ? (int)(m_hitbox.y + m_hitbox.h + .1) : (int)(m_hitbox.y);

        // cout << "blocXY " << blockX << ',' << blockY;
        // cout << "; startXY " << startX << ',' << startY;
        // cout << '\n';

        // Verification horizontale puis verticale
        for (int i = 0; i < blockX; i++) {
            for (int j = 0; j < blockY; j++) {
                if (scene.isSolidTile(startX + i, startY + j * increment)) {
                    // cout << "toucheY" << '\n';
                    hit = 1;
                    blockY = j;

                    replacementPosition = dy > 0 ? startY + j - m_hitbox.h : startY + j * increment + 1;
                    m_hitbox.y = replacementPosition;
                    m_position.moveTo(p.first, m_hitbox.y + m_hitbox.h / 2.);
                }
            }
        }

        if (!hit) {
            m_hitbox.y += dy;
            m_position.moveBy(.0, dy);
        }
    }

    // touche le sol une fois replace ?
    if (hit && (scene.isInsideMap(m_hitbox.x, m_hitbox.y + m_hitbox.h) || scene.isInsideMap(m_hitbox.x + m_hitbox.w - 0.01, m_hitbox.y + m_hitbox.h))) {
        m_touchGround = 1;
        m_flightTime = 0;
    } else m_touchGround = 0;

    // if (dx || dy) {
    //     p = m_position.getCoords();
    //     m_tilePosition.setCoords((int)(p.first), (int)(p.second));
    // }
}

/**
 * @fn void Entity::checkIfVisible (SDL_Rect &camera)
 * @param camera camera
 * @brief verifie si l'entite est presente sur la camera
*/
void Entity::checkIfVisible (SDL_Rect &camera, int tileSize) {
    if ((m_hitbox.x + m_hitbox.w) * tileSize - camera.x > 0 && m_hitbox.x * tileSize - camera.x < camera.w + tileSize && (m_hitbox.y + m_hitbox.h) * tileSize - camera.y > 0 && m_hitbox.y * tileSize - camera.y < camera.h + tileSize) {
        m_visible = true;
    } else {
        m_visible = false;
    }
}

/**
 * @fn void Entity::calculateGlobalPosition ()
 * @returns 
*/
void Entity::calculateGlobalPosition () {
    Entity *e = this->m_parent;
    m_globalPosition = m_position;
    while (e != nullptr) {
        m_globalPosition.moveBy(e->m_position);
        e = e->m_parent;
    }
}







// Operateurs surcharges


bool operator== (Entity const &a, Entity const &b) {
    return a.getId() == b.getId();
}

bool operator!= (Entity const &a, Entity const &b) {
    return a.getId() != b.getId();
}



// classe ShowEntity








ShowEntity::ShowEntity (SDL_Renderer *renderer, Scene *scene, std::vector<SDL_Texture*> *entityTextures)
    : m_renderer(renderer), m_scene(scene), m_entityTextures(entityTextures)
{}


/**
 * @fn void ShowEntity::operator() (Entity &e)
 * @param e entite
 * @brief foncteur d'affichage des entites
*/
void ShowEntity::operator() (Entity &e) {
    // if (e.isVisible())
        // e.show(m_renderer, m_scene->getCameraPos(), *m_entityTextures);
}


