
#include <SDL2/SDL.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

#include "constants.hpp"
#include "Scene.hpp"
#include "texture_management.hpp"
#include "Player.hpp"



using namespace std;

Scene::Scene () : m_mapWidth(0), m_mapHeight(0), m_camera({0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}), m_renderer(nullptr), m_player(nullptr), m_tileSize(TILE_SIZE), m_windowWidth(WINDOW_WIDTH), m_windowHeight(WINDOW_HEIGHT)
{
    m_mapRenderWidth = m_windowWidth / m_tileSize + 1;
    m_mapRenderHeight = m_windowHeight / m_tileSize + 1;

    m_maxAreaWidth = (3 * m_mapRenderWidth / m_tileSize / 4);
    m_maxAreaHeight = 2 * m_mapRenderHeight * m_tileSize / 3;

    m_map = new vector<vector<int>>;
    m_tiles = new vector<SDL_Texture*>;
    m_mobList = new list<Entity>;
    m_blockEntityList = new list<Entity>;
    m_itemDropList = new list<Entity>;
    m_visibleEntities = new list<Entity*>;
    m_mobEntityTextures = new vector<SDL_Texture*>;
    m_blockEntityTextures = new vector<SDL_Texture*>;
    m_itemDropEntityTextures = new vector<SDL_Texture*>;
    m_map->clear();
    m_tiles->clear();
    m_mobList->clear();
    m_itemDropList->clear();
    m_blockEntityList->clear();
    m_visibleEntities->clear();
    m_mobEntityTextures->clear();
    m_blockEntityTextures->clear();
    m_itemDropEntityTextures->clear();

    m_player = new Player;
    initPlayer();

    m_showEntities = new ShowEntity(m_renderer, this, m_mobEntityTextures);
    
    m_collisionIdArray[0] = 2;
    m_collisionIdArray[1] = 3;
}

Scene::Scene(SDL_Renderer *renderer) : m_mapWidth(0), m_mapHeight(0), m_camera({0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}), m_renderer(renderer), m_tileSize(TILE_SIZE), m_windowWidth(WINDOW_WIDTH), m_windowHeight(WINDOW_HEIGHT)
{
    m_mapRenderWidth = m_windowWidth / m_tileSize + 1;
    m_mapRenderHeight = m_windowHeight / m_tileSize + 1;

    m_maxAreaWidth = 3 * m_mapRenderWidth * m_tileSize / 4;
    m_maxAreaHeight = 2 * m_mapRenderHeight * m_tileSize / 3;

    m_map = new vector<vector<int>>;
    m_tiles = new vector<SDL_Texture*>;
    m_mobList = new list<Entity>;
    m_blockEntityList = new list<Entity>;
    m_itemDropList = new list<Entity>;
    m_visibleEntities = new list<Entity*>;
    m_mobEntityTextures = new vector<SDL_Texture*>;
    m_blockEntityTextures = new vector<SDL_Texture*>;
    m_itemDropEntityTextures = new vector<SDL_Texture*>;
    m_map->clear();
    m_tiles->clear();
    m_mobList->clear();
    m_itemDropList->clear();
    m_blockEntityList->clear();
    m_visibleEntities->clear();
    m_mobEntityTextures->clear();
    m_blockEntityTextures->clear();
    m_itemDropEntityTextures->clear();

    m_player = new Player;
    initPlayer();

    m_showEntities = new ShowEntity(m_renderer, this, m_mobEntityTextures);

    m_collisionIdArray[0] = 2;
    m_collisionIdArray[1] = 3;
}

Scene::~Scene () {
    unloadMap();
    m_tiles->clear();
    m_mobList->clear();
    m_itemDropList->clear();
    m_blockEntityList->clear();
    delete m_map;
    delete m_tiles;
    delete m_mobList;
    delete m_blockEntityList;
    delete m_itemDropList;

    delete m_mobEntityTextures;
    delete m_blockEntityTextures;
    delete m_itemDropEntityTextures;
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

Player *Scene::getPlayer () const {
    return m_player;
}

std::vector<SDL_Texture*> *Scene::getMobEntityTextures () const {
    return m_mobEntityTextures;
}

int Scene::getTileSize () const {
    return m_tileSize;
}

void Scene::setTileSize (int value) {
    m_tileSize = value;
}

void Scene::increaseTileSize () {
    m_tileSize++;
}

void Scene::decreaseTileSize () {
    m_tileSize--;
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
 * @fn int Scene::loadTiles (SDL_Renderer *renderer)
 * @param renderer renderer
 * @returns 0 en cas de succes
 * @brief charge les tuiles dans la liste des textures de la carte
*/
int Scene::loadTiles () {
    string tileFile = "";
    SDL_Texture *tile;
    int i = 0;
    ostringstream stream;
    while (1) {
        i++;
        stream.str("");
        stream << "ressources/fg" << i << ".bmp";
        tileFile = stream.str();
        tile = loadTexture(m_renderer, tileFile);
        if (tile) m_tiles->push_back(tile);
        else break;
    }
    return 0;
}

/**
 * @fn void Scene::unloadTiles ()
 * @brief decharge les tuiles de la carte
*/
void Scene::unloadTiles () {
    for (int i = 0; i < TILES_TEXTURE_COUNT; i++) {
        SDL_DestroyTexture(m_tiles->at(i));
    }
    m_tiles->clear();
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
    Entity e;
    SDL_FRect h = {0, 0, 1., 1.};
    string fileName = "data/entities/entities";
    string entityName = "";
    int value;
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
        m_mobList->push_front(e);
        Entity::increaseCount();
        cout << entityName << endl;
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


int Scene::loadEntityTextures () {
    loadMobEntityTextures();
    loadBlockEntityTextures();
    loadItemDropEntityTextures();
    return 0;
}

int Scene::loadMobEntityTextures () {
    string fileName = "";
    ostringstream stream;
    SDL_Texture *texture = NULL;
    int i = 0;

    while (1) {
        i++;
        stream.str("");
        stream << "ressources/entity" << i << ".bmp";
        fileName = stream.str();
        texture = loadTexture(m_renderer, fileName);
        if (texture) m_mobEntityTextures->push_back(texture);
        else break;
    }
    return 0;
}

int Scene::loadBlockEntityTextures () {
    return 0;
}

int Scene::loadItemDropEntityTextures () {
    return 0;
}


void Scene::unloadEntityTextures () {
    unloadMobEntityTextures();
    unloadBlockEntityTextures();
    unloadItemDropEntityTextures();
}
void Scene::unloadMobEntityTextures () {
    for (int i = 0; i < ENTITY_TEXTURE_COUNT; i++) {
        SDL_DestroyTexture(m_mobEntityTextures->at(i));
    }
    m_mobEntityTextures->clear();
}
void Scene::unloadBlockEntityTextures () {
    m_mobEntityTextures->clear();
}
void Scene::unloadItemDropEntityTextures () {
    m_mobEntityTextures->clear();
}











/**
 * @fn void Scene::addEntityToPlayerPos ()
 * @brief ajoute une entite a la position du joueur
*/
void Scene::addEntityToPlayerPos () {
    Entity e;
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

/**
 * @fn void Scene::doActions (SDL_Renderer *renderer, Entity &entity)
 * @brief fait les actions de la scene
 * @see Scene::moveCamera
*/
void Scene::doActions (bool left, bool right, bool up, bool down, unsigned int dt) {
    moveCamera(*m_player->getTarget());

    m_player->getTarget()->move(*this, left, right, up, down, dt, m_renderer);
    // m_player->moveToWorld(*this, m_renderer);


    m_player->doActions(*m_mobList, dt);


}

/**
 * @fn void Scene::moveCamera (Entity &entity)
 * @param target entite cible
 * @brief Deplace la camera vers l'entite cible
*/
void Scene::moveCamera (const Entity &target) {
    SDL_FRect hitbox = target.getHitbox();
    SDL_FRect screenPosition;

    screenPosition.x = hitbox.x * m_tileSize - m_camera.x;
    screenPosition.y = hitbox.y * m_tileSize - m_camera.y;
    screenPosition.w = hitbox.w * m_tileSize;
    screenPosition.h = hitbox.h * m_tileSize;

    // droite
    if (screenPosition.x + screenPosition.w > m_maxAreaWidth) {
        m_camera.x = hitbox.x * m_tileSize - m_maxAreaWidth;
    }
    // gauche
    else if (screenPosition.x < m_mapRenderWidth - m_maxAreaWidth) {
        m_camera.x = hitbox.x  * m_tileSize- (m_mapRenderWidth * m_tileSize - m_maxAreaWidth);
    }

    // bas
    if (screenPosition.y + screenPosition.h > m_maxAreaHeight) {
        m_camera.y = hitbox.y * m_tileSize - m_maxAreaHeight;
    }
    // haut
    else if (screenPosition.y < m_mapRenderHeight - m_maxAreaHeight) {
        m_camera.y = hitbox.y * m_tileSize - (m_mapRenderHeight * m_tileSize - m_maxAreaHeight);
    }

    m_camera.x = min<int>(max<int>(m_camera.x, 0), (m_mapWidth - m_mapRenderWidth + 1) * m_tileSize);
    m_camera.y = min<int>(max<int>(m_camera.y, 0), (m_mapHeight - m_mapRenderHeight + 1) * m_tileSize);

    for (std::list<Entity>::iterator it = m_mobList->begin(); it != m_mobList->end(); it++) {
        (*it).checkIfVisible(m_camera, m_tileSize);
    }
}


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

/**
 * @fn SDL_Point Scene::getCameraPos () const
 * @returns la position de la camera
 * @brief retourne la position de la camera
*/
SDL_Rect Scene::getCameraPos () const {
    return m_camera;
}

/**
 * @fn void Scene::setCameraPos (int x, int y)
 * @param x position x
 * @param x position x
 * @brief deplace la camera
*/
void Scene::setCameraPos (int x, int y) {
    m_camera.x = x;
    m_camera.y = y;
}



/**
 * @fn void Scene::displayScene () const
 * @brief affiche toute la scene
*/
void Scene::displayScene () const {
    displayMap();
    // for_each(m_mobList->begin(), m_mobList->end(), *m_showEntities);
    for (std::list<Entity>::iterator it = m_mobList->begin(); it != m_mobList->end(); it++) {
        if ((*it).isVisible())
            (*it).show(m_renderer, m_camera, m_tileSize, *m_mobEntityTextures);
    }
    m_player->getTarget()->show(m_renderer, m_camera, m_tileSize, *m_mobEntityTextures);
}

/**
 * @fn void Scene::displayMap (SDL_Renderer *renderer) const
 * @param renderer renderer
 * @brief affiche la carte
*/
void Scene::displayMap () const {
    SDL_Rect rect = {0, 0, m_tileSize, m_tileSize};
    int x1 = 0, x2 = 0, y1 = 0, y2 = 0, mx = 0, my = 0;

    // coordonees reelles de la zone de rendu

    // reste de la division, on veut commencer en dehors de l'ecran
    x1 = m_camera.x % m_tileSize * -1;
    x2 = x1 + m_mapRenderWidth * m_tileSize + (x1 == 0 ? 0 : m_tileSize);

    // reste de la division, on veut commencer en dehors de l'ecran
    y1 = m_camera.y % m_tileSize * -1;
    y2 = y1 + m_mapRenderHeight * m_tileSize + (y1 == 0 ? 0 : m_tileSize);

    mx = m_camera.x / m_tileSize;
    my = m_camera.y / m_tileSize;

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
        mx = m_camera.x / m_tileSize;
        my++;
    }
}



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
    Entity e;
    m_mobList->push_front(e);
    m_player->setTarget(&m_mobList->front());
    
    SDL_FRect prect = {5, 26, 1, 1};
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
