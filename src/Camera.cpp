
#include <SDL2/SDL.h>
#include <utility>
#include <vector>
#include <iterator>
#include <string>
#include <sstream>

#include <cmath>



#include "Camera.hpp"
#include "App.hpp"
#include "Vector.hpp"
#include "Entity.hpp"


using namespace std;

SDL_Renderer *Camera::m_renderer = nullptr;
vector<SDL_Texture*> *Camera::m_tiles = new vector<SDL_Texture*>;
vector<SDL_Texture*> *Camera::m_mobEntityTextures = new vector<SDL_Texture*>;

Camera::Camera () : m_position(0, 0) {

    m_lockedX = 0;
    m_lockedY = 0;
}

Camera::Camera (float sceneWidth, float sceneHeight, float windowWidth, float windowHeight, SDL_Renderer *renderer) : m_position(0, 0), m_sceneWidth(sceneWidth), m_sceneHeight(sceneHeight), m_screenWidth(windowWidth), m_screenHeight(windowHeight)
{
    m_tileSize = m_screenWidth / m_sceneWidth;
    m_renderer = renderer;

    m_lockedX = 0;
    m_lockedY = 0;

    m_cameraMode = TargetEntity;

}

Camera::~Camera () {
    // if (m_tiles)
    // delete m_tiles;
    // if (m_mobEntityTextures)
    // delete m_mobEntityTextures;
}

/**
 * @fn const Vector2D Camera::getPosition () const
 * @returns La position de la camera sur la scene
 * @brief Donne la position de la camera sur la scene
*/
const Vector2D Camera::getPosition () const {
    return m_position;
}

/**
 * @fn const SDL_FRect Camera::getBoundingBox () const
 * @returns L'aire couverte par la camera
 * @brief Donne l'aire couverte par la camera
*/
const SDL_FRect Camera::getBoundingBox () const {
    pair<float, float> camCoords = m_position.getCoords();
    SDL_FRect boundingBox = {
    (float)(camCoords.first - m_sceneWidth / 2.), 
    (float)(camCoords.second - m_sceneHeight / 2.), 
    m_sceneWidth, 
    m_sceneHeight
    };
    return boundingBox;
}


// float Camera::getTileSize () const {
//     return m_tileSize * m_zoom;
// }

/**
 * @fn void Camera::moveBy (const Vector2D &vector)
 * @param vector Vecteur de deplacement
 * @brief Decale la camera de vector
*/
void Camera::moveBy (const Vector2D &vector) {
    if (m_cameraMode == Free) {
        m_position.moveBy(vector);
    } else if (m_cameraMode == TargetEntity) {
        // deplacer la camera sans quitter l'entite
        if (!m_lockedX) {
            // bouger X
        }
        if (!m_lockedY) {
            // bouger Y
        }
    }

    snapToMap();

    // m_boundingBox.x += x;
    // m_boundingBox.y += y;
}

/**
 * @fn void Camera::moveBy (float x, float y)
 * @param x x
 * @param y y
 * @brief Decale la camera de vector
*/
void Camera::moveBy (float x, float y) {
    if (m_cameraMode == Free) {
        m_position.moveBy(x, y);
    } else if (m_cameraMode == TargetEntity) {
        // deplacer la camera sans quitter l'entite
        if (!m_lockedX) {
            m_position.moveBy(0, y);
        }
        if (!m_lockedY) {
            m_position.moveBy(x, 0);
        }

        snapToMap();
    }

    

    // m_boundingBox.x += x;
    // m_boundingBox.y += y;
}

/**
 * @fn void Camera::moveTo (float x, float y)
 * @param x Position x
 * @param y Position y
 * @brief Deplace la camera vers un point de la carte
*/
void Camera::moveTo (float x, float y) {
    m_position.moveTo(x, y);
    // m_boundingBox.x = x - m_boundingBox.w / 2;
    // m_boundingBox.y = y - m_boundingBox.h / 2;

    snapToMap();
}

void Camera::move () {

}

/**
 * @fn void Camera::snapToMap ()
 * @brief Verifie si l'aire de la camera est integralement sur la carte, la camera est deplacee si besoin
*/
void Camera::snapToMap () {
    pair<float, float> camCoords = m_position.getCoords();
    if (camCoords.first - m_sceneWidth / 2. < 0) {
        camCoords.first = m_sceneWidth / 2.;
    } else if (camCoords.first + m_sceneWidth / 2. > 100) {
        camCoords.first = 100 - m_sceneWidth / 2.;
    }

    if (camCoords.second - m_sceneHeight / 2. < 0) {
        camCoords.second = m_sceneHeight / 2.;
    } else if (camCoords.second + m_sceneHeight / 2. > 70) {
        camCoords.second = 70 - m_sceneHeight / 2.;
    }

    m_position.moveTo(camCoords.first, camCoords.second);
}

/**
 * @fn void Camera::centerToTarget ()
 * @brief Centre la camera sur l'entite cible
 * @see Camera::lockTo
*/
void Camera::centerToTarget () {
    pair<float, float> ePos = m_target->getPosition().getCoords();
    pair<float, float> camPos = m_position.getCoords();

    if (m_cameraMode != Locked) {
        if (!m_lockedX) {
            camPos.first = ePos.first;
        }
        if (!m_lockedY) {
            camPos.second = ePos.second;
        }
        m_position.setCoords(camPos.first, camPos.second);
        snapToMap();
    }
}


void Camera::setTarget (Entity *e) {
    m_target = e;
}


void Camera::setMode (CameraMode mode) {
    m_cameraMode = mode;
}

void Camera::setRenderer (SDL_Renderer *renderer) {
    m_renderer = renderer;
}

/**
 * @fn void Camera::lock ()
 * @brief Bloque la camera sur la scene
*/
void Camera::lock () {
    m_cameraMode = Locked;
}

/**
 * @fn void Camera::lockX ()
 * @brief Bloque la camera sur l'axe X
*/
void Camera::lockX () {
    m_lockedX = true;
}

/**
 * @fn void Camera::lockY ()
 * @brief Bloque la camera sur l'axe Y
*/
void Camera::lockY () {
    m_lockedY = true;
}


/**
 * @fn void Camera::unlock ()
 * @brief Debloque la camera
*/
void Camera::unlock () {
    m_cameraMode = Free;
}

/**
 * @fn void Camera::unlockX ()
 * @brief Debloque la camera sur l'axe X
*/
void Camera::unlockX () {
    m_lockedX = false;
}

/**
 * @fn void Camera::unlockY ()
 * @brief Debloque la camera sur l'axe Y
*/
void Camera::unlockY () {
    m_lockedY = false;
}

/**
 * @fn void Camera::lockTo (Entity &entity)
 * @param entity Entite a suivre
 * @brief Definit l'entite comme cible a suivre
*/
void Camera::lockTo (Entity &entity) {
    m_cameraMode = LockEntity;
    m_target = &entity;
    centerToTarget();
    
}

/**
 * @fn void Camera::setWindowDimension (int width, int height)
 * @param width Largeur
 * @param height Hauteur
 * @brief Definit la dimension de la fenetre
*/
void Camera::setWindowDimension (int width, int height) {
    m_screenWidth = width;
    m_screenHeight = height;
    m_tileSize = m_screenWidth / m_sceneWidth;
    snapToMap();
}

/**
 * @fn void Camera::setSceneDimension (float width, float height)
 * @param width Largeur
 * @param height Hauteur
 * @brief Definit la dimension de la camera sur la scene
*/
void Camera::setSceneDimension (float width, float height) {
    m_sceneWidth = width;
    m_sceneHeight = height;
    m_tileSize = m_screenWidth / m_sceneWidth;
    snapToMap();
}

/**
 * @fn void Camera::update ()
 * @brief Met a jour les informations de la camera 
*/
void Camera::update () {
    if (m_cameraMode == LockEntity)
        centerToTarget();
    if (m_cameraMode == TargetEntity)
        followTarget();
}

/**
 * 
*/
void Camera::setTargetScreenArea (float ratioX, float ratioY) {
    if (ratioX >= 0. && ratioX <= 1.) {
        m_targetScreenArea.w = m_sceneWidth * ratioX;
        m_targetScreenArea.x = - m_targetScreenArea.w / 2.;
    } else {
        m_targetScreenArea.w = CAMERA_DEFAULT_AREA_WIDTH;
        m_targetScreenArea.x = - m_targetScreenArea.w / 2.;
    }
    if (ratioY >= 0. && ratioY <= 1.) {
        m_targetScreenArea.h = m_sceneHeight * ratioY;
        m_targetScreenArea.y = - m_targetScreenArea.h / 2.;
    } else {
        m_targetScreenArea.h = CAMERA_DEFAULT_AREA_HEIGHT;
        m_targetScreenArea.y = - m_targetScreenArea.h / 2.;
    }

}

void Camera::followTarget () {

    if (m_cameraMode == Locked)
        return;
    
    pair<float, float> campos = m_position.getCoords();
    pair<float, float> ppos = m_target->getPosition().getCoords();
    float leftSide = campos.first + m_targetScreenArea.x;
    float rightSide = leftSide + m_targetScreenArea.w;
    float upSide = campos.second + m_targetScreenArea.y;
    float downSide = upSide + m_targetScreenArea.h;
    // cout << leftSide;
    // trop a gauche
    if (ppos.first < leftSide) {
        // cout << "hors de la zone" << endl;
        m_position.moveBy(ppos.first - leftSide, 0.);
    }
    // trop a droite
    else if (ppos.first > rightSide) {
        m_position.moveBy(ppos.first - rightSide, 0.);
    }
    // trop haut
    if (ppos.second < upSide) {
        // cout << "hors de la zone" << endl;
        m_position.moveBy(0., ppos.second - upSide);
    }
    // trop a bas
    else if (ppos.second > downSide) {
        m_position.moveBy(0., ppos.second - downSide);
    }

    snapToMap();

}


// Methodes statiques

/**
 * @fn void Camera::loadEntityTextures (const char *fileName)
 * @param fileName Patterne du fichier source
 * @brief Charge les textures des entites
*/
void Camera::loadEntityTextures (const char *fileName) {
    // m_mobEntityTextures->resize(ENTITY_TEXTURE_COUNT + 1);
    string tileFile = "";
    SDL_Texture *tile;
    int i = 0;
    ostringstream stream;

    // SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

    while (i < ENTITY_TEXTURE_COUNT) {
        i++;
        stream.str("");
        stream << fileName << i << ".bmp";
        tileFile = stream.str();
        tile = App::loadTexture(tileFile.c_str());
        if (tile) m_mobEntityTextures->push_back(tile);
        else break;
    }


}

/**
 * @fn void Camera::loadTilesTextures (const char *fileName)
 * @param fileName Patterne du fichier source
 * @brief Charge les textures des tuiles de la carte
*/
void Camera::loadTilesTextures (const char *fileName) {
    // m_tiles->resize(TILES_TEXTURE_COUNT + 1);
    string tileFile = "";
    SDL_Texture *tile;
    int i = 0;
    ostringstream stream;

    while (i < TILES_TEXTURE_COUNT) {
        i++;
        stream.str("");
        stream << fileName << i << ".bmp";
        tileFile = stream.str();
        tile = App::loadTexture(tileFile.c_str());
        if (tile) m_tiles->push_back(tile);
        else break;
    }
}

/**
 * @fn void Camera::unloadEntityTextures ()
 * @brief Decharge les textures des entites
*/
void Camera::unloadEntityTextures () {
    if (m_mobEntityTextures) {
        for (int i = 0; i < ENTITY_TEXTURE_COUNT; i++) {
            SDL_DestroyTexture(m_mobEntityTextures->at(i));
        }
        m_mobEntityTextures->clear();
    }
}

/**
 * @fn void Camera::unloadTilesTextures ()
 * @brief Decharge les textures des tuiles
*/
void Camera::unloadTilesTextures () {
    if (m_tiles) {
        for (int i = 0; i < TILES_TEXTURE_COUNT; i++) {
            SDL_DestroyTexture(m_tiles->at(i));
        }
        m_tiles->clear();
    }
}



/**
 * @fn void Camera::displayMap (const vector<vector<int>> *map)
 * @param map Carte de la scene
 * @brief Affiche la carte sur l'ecran
*/
void Camera::displayMap (const vector<vector<int>> *map) {

    SDL_FRect rect = {0, 0, m_tileSize, m_tileSize};
    pair<float, float> cameraPos = m_position.getCoords();

    // m_tileSize = m_screenWidth / m_sceneWidth;

    // coordonees des points de debut et de fin du rectangle sur l'ecran
    float x1;
    float x2;
    float y1;
    float y2;

    // coordonees sur la carte
    int mx = 0;
    int my = 0;

    int id;

    rect.w = m_tileSize;
    rect.h = m_tileSize;



    x1 = ((cameraPos.first - m_sceneWidth / 2.) - truncf(cameraPos.first - m_sceneWidth / 2.)) * m_tileSize * -1;
    x2 = x1 + m_screenWidth + (!x1 ? 0 : m_tileSize);

    y1 = ((cameraPos.second - m_sceneHeight / 2.) - truncf(cameraPos.second - m_sceneHeight / 2.)) * m_tileSize * -1;
    y2 = y1 + m_screenHeight + (!y1 ? 0 : m_tileSize);

    mx = (int)(cameraPos.first - m_sceneWidth / 2.);
    my = (int)(cameraPos.second - m_sceneHeight / 2.);


    for (float y = y1; y < y2; y += m_tileSize) {
        for (float x = x1; x < x2; x += m_tileSize) {
            id = (my >= 0 && my < map->size() && mx >= 0 && mx < map->at(my).size()) ? map->at(my).at(mx) : -1;
            rect.x = x;
            rect.y = y;
            if (id > 0) {
                SDL_RenderCopyF(m_renderer, m_tiles->at(id - 1), NULL, &rect);
            }
            mx++;
        }
        mx = (int)(cameraPos.first - m_sceneWidth / 2.);
        my++;
    }
}



/**
 * @fn void Camera::displayEntities (const list<Entity> *entityList)
 * @param entityList Liste des entites
 * @brief Affiche les entites sur l'ecran
*/
void Camera::displayEntities (const list<Entity> *entityList) {
    SDL_FRect rect; // sur l'ecran
    rect.w = m_tileSize;
    rect.h = m_tileSize;
    pair<float, float> ePos; // sur la carte
    SDL_FRect erect; // sur la carte
    pair<float, float> camPos = m_position.getCoords(); // sur la carte

    for (list<Entity>::const_iterator it = entityList->begin(); it != entityList->end(); it++) {
        ePos = (*it).getPosition().getCoords();
        erect = (*it).getHitbox();

        rect.x = ((ePos.first - erect.w / 2.) - (camPos.first - m_sceneWidth / 2.)) * m_tileSize;
        rect.y = ((ePos.second - erect.h / 2.) - (camPos.second - m_sceneHeight / 2.)) * m_tileSize;

        SDL_RenderCopyF(m_renderer, m_mobEntityTextures->at((*it).getTextureId()), NULL, &rect);
    }
}
