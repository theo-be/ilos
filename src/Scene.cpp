
#include <SDL2/SDL.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

#include "Scene.hpp"
#include "App.hpp"
#include "Player.hpp"

#define DEFAULT_MAP_LOCATION "data/maps/map"


using namespace std;

Scene::Scene () : m_mapWidth(0), m_mapHeight(0), m_player(nullptr)
{

    m_map = new vector<vector<int>>;
    m_mobList = new list<Entity>;
    m_blockEntityList = new list<Entity>;
    m_itemDropList = new list<Entity>;
    m_visibleEntities = new list<Entity*>;
    m_map->clear();
    m_mobList->clear();
    m_itemDropList->clear();
    m_blockEntityList->clear();
    m_visibleEntities->clear();

    m_player = new Player;

}


Scene::~Scene () {
    unloadMap();
    m_mobList->clear();
    m_itemDropList->clear();
    m_blockEntityList->clear();
    delete m_map;
    delete m_mobList;
    delete m_blockEntityList;
    delete m_itemDropList;

}

void Scene::init() {
    // m_mapRenderWidth = m_windowWidth / m_tileSize + 1;
    // m_mapRenderHeight = m_windowHeight / m_tileSize + 1;

    // m_maxAreaWidth = 3 * m_mapRenderWidth * m_tileSize / 4;
    // m_maxAreaHeight = 2 * m_mapRenderHeight * m_tileSize / 3;
    
    initPlayer();
    initMap(DEFAULT_MAP_LOCATION);
    loadEntities();

    m_collisionIdArray[0] = 2;
    m_collisionIdArray[1] = 3;
}


list<Entity> *Scene::getMobList() const {
    return m_mobList;
}

list<Entity> *Scene::getBlockEntityList() const {
    return m_blockEntityList;
}

list<Entity> *Scene::getItemDropList() const {
    return m_itemDropList;
}

const std::vector<std::vector<int>> *Scene::getMap () const {
    return m_map;
}

Player *Scene::getPlayer () const {
    return m_player;
}





/**
 * @fn int Scene::initMap (const string &fileName)
 * @param fileName nom du fichier
 * @returns 0 en cas de succes
 * @brief charge la carte 
*/
int Scene::initMap (const char *fileName) {
    m_mapHeight = 0;
    m_mapWidth = 0;

    cout << "Carte reinitialisee" << endl;
    cout << "Chargement de " << fileName << endl;

    ifstream file;
    file.open(fileName);
    file >> m_mapHeight;
    file >> m_mapWidth;
    cout << "Largeur : " << m_mapHeight << endl;
    cout << "Longueur : " << m_mapWidth << endl;

    int v = -1;
    

    m_map->clear();
    m_map->resize(m_mapHeight);
    for (int i = 0; i < m_mapHeight; i++) {
        m_map->push_back(std::vector<int>(m_mapWidth));
        for (int j = 0; j < m_mapWidth; j++) {
            file >> v;
            m_map->at(i).push_back(v);
        }
    }


    file.close();
    cout << "Carte chargee" << endl;
    cout << "Taille des tableaux : " << m_map->size() << "x" << m_map->front().size() << '\n';

    return 0;
}

/**
 * @fn void Scene::unloadMap ()
 * @brief decharge la carte
*/
void Scene::unloadMap () {
    if (m_map->size() <= 0) return;
    for (int i = 0; i < m_mapHeight; i++) {
        m_map->at(i).clear();
    }
    m_map->clear();
}



/**
 * @fn int loadEntities ()
 * @returns 0 en cas de succes
 * @brief charge les entites des fichiers de sauvegarde
*/
int Scene::loadEntities () {
    loadMobEntities();
    loadBlockEntities();
    loadItemDropEntities();
    return 0;
}

/**
 * @fn int Scene::loadMobEntities ()
 * @returns 0 en cas de succes
 * @brief charge les entites Mob du fichier de sauvegarde
*/
int Scene::loadMobEntities () {
    ifstream file;
    Entity e(this);
    SDL_FRect h = {0, 0, 1., 1.};
    string fileName = "data/entities/entities";
    string entityName = "";
    int value;
    Vector2D v;
    float posX = 0.;
    float posY = 0.;
    // stringstream stream;
    file.open(fileName);
    if (!file) cerr << "ERREUR ouverture " << fileName << endl;
    // bool read = true;
    // string s;
    

    // NOM X Y PV PASSIVE TID
    while (!file.fail()) {
        // getline(file, s);
        // stream.clear();
        // stream << s;
        // if (stream.str() == "") {
        //     read = false;
        // }
        // cout << stream.str();
        
        file >> entityName;
        if (entityName == "DEFAULT") break;
        e.setName(entityName);
        file >> value;
        h.x = value;
        file >> value;
        h.y = value;
        e.setHitbox(h);
        file >> value;
        e.setHp(value);
        file >> value;
        e.setPassive(value);
        file >> value;
        e.setTextureId(value);
        posX = h.x + h.w / 2.;
        posY = h.y + h.h / 2.;
        v.setCoords(posX, posY);
        e.setPosition(v);
        m_mobList->push_front(e);
        Entity::increaseCount();
        // cout << entityName << endl;
        entityName = "DEFAULT";
    }

    file.close();
    return 0;
}

int Scene::loadBlockEntities () {
    return 0;
}

int Scene::loadItemDropEntities () {
    return 0;
}

void Scene::unloadEntities () {
    unloadMobEntities();
    unloadBlockEntities();
    unloadItemDropEntities();
}

void Scene::unloadMobEntities () {
    m_mobList->clear();
}

void Scene::unloadBlockEntities () {
    m_blockEntityList->clear();
}

void Scene::unloadItemDropEntities () {
    m_itemDropList->clear();
}







/**
 * @fn void Scene::addEntityToPlayerPos ()
 * @brief ajoute une entite a la position du joueur
*/
void Scene::addEntityToPlayerPos () {
    Entity e(this);
    ostringstream stream;
    stream << "PEDRO" << e.getId();
    string name = stream.str();
    SDL_FRect h = m_player->getTarget()->getHitbox();
    e.setName(name);
    e.setHitbox(h);
    e.setTextureId(0);
    e.setPassive(false);
    m_mobList->push_front(e);
    // On incremente car l'entite va etre detruite a la fin du bloc
    Entity::increaseCount();
}

// /**
//  * @fn void Scene::doActions (SDL_Renderer *renderer, Entity &entity)
//  * @brief fait les actions de la scene
//  * @see Scene::moveCamera
// */
// void Scene::doActions (bool left, bool right, bool up, bool down, unsigned int dt) {
//     moveCamera(*m_player->getTarget());

//     m_player->getTarget()->move(*this, left, right, up, down, dt, m_renderer);
//     // m_player->moveToWorld(*this, m_renderer);


//     m_player->doActions(*m_mobList, dt);


// }

// /**
//  * @fn void Scene::moveCamera (Entity &entity)
//  * @param target entite cible
//  * @brief Deplace la camera vers l'entite cible
// */
// void Scene::moveCamera (const Entity &target) {

//     // ancien systeme de camera

//     SDL_FRect hitbox = target.getHitbox();
//     SDL_FRect screenPosition;

//     screenPosition.x = hitbox.x * m_tileSize - m_cameraRect.x;
//     screenPosition.y = hitbox.y * m_tileSize - m_cameraRect.y;
//     screenPosition.w = hitbox.w * m_tileSize;
//     screenPosition.h = hitbox.h * m_tileSize;

//     // droite
//     if (screenPosition.x + screenPosition.w > m_maxAreaWidth) {
//         m_cameraRect.x = hitbox.x * m_tileSize - m_maxAreaWidth;
//     }
//     // gauche
//     else if (screenPosition.x < m_mapRenderWidth - m_maxAreaWidth) {
//         m_cameraRect.x = hitbox.x  * m_tileSize- (m_mapRenderWidth * m_tileSize - m_maxAreaWidth);
//     }

//     // bas
//     if (screenPosition.y + screenPosition.h > m_maxAreaHeight) {
//         m_cameraRect.y = hitbox.y * m_tileSize - m_maxAreaHeight;
//     }
//     // haut
//     else if (screenPosition.y < m_mapRenderHeight - m_maxAreaHeight) {
//         m_cameraRect.y = hitbox.y * m_tileSize - (m_mapRenderHeight * m_tileSize - m_maxAreaHeight);
//     }

//     m_cameraRect.x = min<int>(max<int>(m_cameraRect.x, 0), (m_mapWidth - m_mapRenderWidth + 1) * m_tileSize);
//     m_cameraRect.y = min<int>(max<int>(m_cameraRect.y, 0), (m_mapHeight - m_mapRenderHeight + 1) * m_tileSize);

//     for (std::list<Entity>::iterator it = m_mobList->begin(); it != m_mobList->end(); it++) {
//         (*it).checkIfVisible(m_cameraRect, m_tileSize);
//     }


//     // nouveau systeme de camera

//     // float tileSize = m_camera->getTileSize();
//     // pair<float, float> playerPosition = target.getPosition().getCoords();

//     // SDL_Point positionOnScreen = {
//     //     playerPosition.first * tileSize,
//     //     playerPosition.second * tileSize
//     // };

//     pair<float, float> eCoords = target.getPosition().getCoords();
//     m_camera->moveTo(eCoords.first, eCoords.second);


// }


/**
 * @fn bool Scene::isInsideMap (int x, int y) const
 * @param x position x globale
 * @param y position y globale
 * @returns true si l'entite est dans la carte, false sinon
*/
bool Scene::isInsideMap (float x, float y) const {
    if (x >= m_mapWidth || y >= m_mapHeight) return false;
    if (x < 0 || y < 0) return false;
    for (int i = 0; i < SOLID_TILES_COUNT; i++) {
        if (m_map->at((int)y).at((int)x) == m_collisionIdArray[i]) return true;
    }
    return false;
}

/**
 * @fn bool Scene::isOOB (int x, int y) const
 * @param x position x globale
 * @param y position y globale
 * @returns true si l'entite est dans la carte, false sinon
*/
bool Scene::isOOB (int x, int y) const {
    if (x >= m_mapWidth || y >= m_mapHeight) return true;
    if (x < 0 || y < 0) return true;
    return false;
}

/**
 * @fn bool Scene::isSolidTile (int blockX, int blockY) const
 * @param blockX position x
 * @param blockY position y
 * @returns true si la tuile est solide, false sinon
*/
bool Scene::isSolidTile (int blockX, int blockY) const {
    if (blockX >= m_mapWidth || blockY >= m_mapHeight) return true;
    if (blockX < 0 || blockY < 0) return true;
    for (int i = 0; i < SOLID_TILES_COUNT; i++) {
        if (m_map->at(blockY).at(blockX) == m_collisionIdArray[i]) return true;
    }
    return false;
}

/**
 * @fn int Scene::getTileAt (int x, int y) const
 * @param x position x de la tuile
 * @param y position y de la tuile
 * @returns la valeur de la tuile
 * @brief recupere le numero de la tuile
*/
int Scene::getTileAt (int x, int y) const {
    return m_map->at(y).at(x);
}

/**
 * @fn pair<int, int> Scene::getMapDim () const
 * @returns la dimension de la carte
 * @brief retourne la dimension de la carte
*/
pair<int, int> Scene::getMapDim () const {
    pair<int, int> dim;
    dim.first = m_mapWidth;
    dim.second = m_mapHeight;
    return dim;
}

// /**
//  * @fn SDL_Point Scene::getCameraPos () const
//  * @returns la position de la camera
//  * @brief retourne la position de la camera
// */
// SDL_Rect Scene::getCameraPos () const {
//     return m_cameraRect;
// }

// /**
//  * @fn void Scene::setCameraPos (int x, int y)
//  * @param x position x
//  * @param y position y
//  * @brief deplace la camera
// */
// void Scene::setCameraPos (int x, int y) {
//     m_cameraRect.x = x;
//     m_cameraRect.y = y;
// }



// /**
//  * @fn void Scene::displayScene () const
//  * @brief affiche toute la scene
// */
// void Scene::displayScene () const {
//     displayMap();
//     // for_each(m_mobList->begin(), m_mobList->end(), *m_showEntities);
//     // for (std::list<Entity>::iterator it = m_mobList->begin(); it != m_mobList->end(); it++) {
//     //     if ((*it).isVisible())
//     //         (*it).show(m_renderer, m_cameraRect, m_tileSize, *m_mobEntityTextures);
//     // }
//     // m_player->getTarget()->show(m_renderer, m_cameraRect, m_tileSize, *m_mobEntityTextures);

//     m_camera->drawE(m_renderer, m_mobList, m_mobEntityTextures);

// }

// /**
//  * @fn void Scene::displayMap (SDL_Renderer *renderer) const
//  * @param renderer renderer
//  * @brief affiche la carte
// */
// void Scene::displayMap () const {
/* 
    // ancien systeme
    SDL_Rect rect = {0, 0, m_tileSize, m_tileSize};
    int x1 = 0, x2 = 0, y1 = 0, y2 = 0, mx = 0, my = 0;

    // coordonees reelles de la zone de rendu

    // reste de la division, on veut commencer en dehors de l'ecran
    x1 = m_cameraRect.x % m_tileSize * -1;
    x2 = x1 + m_mapRenderWidth * m_tileSize + (x1 == 0 ? 0 : m_tileSize);

    // reste de la division, on veut commencer en dehors de l'ecran
    y1 = m_cameraRect.y % m_tileSize * -1;
    y2 = y1 + m_mapRenderHeight * m_tileSize + (y1 == 0 ? 0 : m_tileSize);

    mx = m_cameraRect.x / m_tileSize;
    my = m_cameraRect.y / m_tileSize;

    for (int y = y1; y < y2; y+= m_tileSize) {
        for (int x = x1; x < x2; x += m_tileSize) {
            if (mx >= 0 && my >= 0 && mx < m_mapWidth && my < m_mapHeight) {
                int id = m_map->at(my).at(mx);
                rect.x = x;
                rect.y = y;
                if (id > 0) {
                    SDL_RenderCopy(m_renderer, m_tiles->at(id - 1), NULL, &rect);
                }
            }
            mx++;
        }
        mx = m_cameraRect.x / m_tileSize;
        my++;
    }
 */

    // nouveau systeme

//     m_camera->draw(m_renderer, m_tiles, m_map);


// }



/**
 * @fn void Scene::addVisibleEntity (Entity *e)
 * @param e Entite a ajouter
 * @brief Ajoute l'entite a la liste des entites a afficher a la camera
*/
void Scene::addVisibleEntity (Entity *e) {
    for (std::list<Entity*>::iterator it = m_visibleEntities->begin(); it != m_visibleEntities->end(); it++) {
        if (**it == *e) return;
    }
    m_visibleEntities->push_front(e);
}




/**
 * @fn void Scene::removeVisibleEntity (Entity *e)
 * @param e Entite a supprimer
 * @brief supprime l'entite a la liste des entites a afficher a la camera
*/
void Scene::removeVisibleEntity (Entity *e) {
    m_visibleEntities->remove(e);
}

void Scene::initPlayer () {
    Entity e(this);
    m_mobList->push_front(e);
    m_player->setTarget(&m_mobList->front());
    
    // SDL_FRect prect = {5, 26, 1, 1};
    SDL_FRect prect = {61, 26, 1, 1};
    Vector2D ppos;
    ppos.setCoords(prect.x + prect.w / 2, prect.y + prect.h / 2);
    string pname = "Joueur";

    m_player->getTarget()->setName(pname);
    m_player->getTarget()->setTextureId(3);
    m_player->getTarget()->setHp(10);
    m_player->getTarget()->setHitbox(prect);
    m_player->getTarget()->setPosition(ppos);
    m_player->getTarget()->setPosition0(ppos);
}

void Scene::update () {
    // m_player->getTarget()->move(*this);
    m_player->getTarget()->move();
    m_player->update();
}
