
#include <SDL2/SDL.h>
#include <utility>
#include <vector>
#include <iterator>
#include <string>
#include <sstream>

#include <cmath>

#include "constants.hpp"

#include "Vector.hpp"
#include "Camera.hpp"
#include "Entity.hpp"

#include "texture_management.hpp"

using namespace std;

SDL_Renderer *Camera::m_renderer = nullptr;
vector<SDL_Texture*> *Camera::m_tiles = nullptr;
vector<SDL_Texture*> *Camera::m_mobEntityTextures = nullptr;


Camera::Camera (float x, float y, float sceneWidth, float sceneHeight, float windowWidth, float windowHeight, SDL_Renderer *renderer) : m_position(x, y), m_sceneWidth(sceneWidth), m_sceneHeight(sceneHeight), m_screenWidth(windowWidth), m_screenHeight(windowHeight)
{
    m_tileSize = m_screenWidth / m_sceneWidth;
    m_renderer = renderer;

    m_tiles = new vector<SDL_Texture*>;
    m_mobEntityTextures = new vector<SDL_Texture*>;

    m_lockedX = 0;
    m_lockedY = 0;
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


// ???
float Camera::getTileSize () const {
    return m_tileSize * m_zoom;
}

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


void Camera::setMode (CameraMode mode) {
    m_cameraMode = mode;
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
    m_cameraMode = TargetEntity;
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
 * @brief Met a jour les informations de la camera (par ex : sa position sur l'entite a suivre, etc.)
*/
void Camera::update () {
    // objectif : mettre a jour tout sur la cam directement
    if (m_cameraMode == TargetEntity)
        centerToTarget();

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
        tile = loadTexture(m_renderer, tileFile);
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
        tile = loadTexture(m_renderer, tileFile);
        if (tile) m_tiles->push_back(tile);
        else break;
    }
}

/**
 * @fn void Camera::unloadEntityTextures ()
 * @brief Decharge les textures des entites
*/
void Camera::unloadEntityTextures () {
    for (int i = 0; i < ENTITY_TEXTURE_COUNT; i++) {
        SDL_DestroyTexture(m_mobEntityTextures->at(i));
    }
    delete m_mobEntityTextures;
}

/**
 * @fn void Camera::unloadTilesTextures ()
 * @brief Decharge les textures des tuiles
*/
void Camera::unloadTilesTextures () {
    for (int i = 0; i < TILES_TEXTURE_COUNT; i++) {
        SDL_DestroyTexture(m_tiles->at(i));
    }
    delete m_tiles;
}



/**
 * @fn void Camera::displayMap (const vector<vector<int>> *map)
 * @param map Carte de la scene
 * @brief Affiche la carte sur l'ecran
*/
void Camera::displayMap (const vector<vector<int>> *map) {

    snapToMap();
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

    rect.w = m_tileSize;
    rect.h = m_tileSize;



    x1 = ((cameraPos.first - m_sceneWidth / 2.) - truncf(cameraPos.first - m_sceneWidth / 2.)) * m_tileSize * -1;
    x2 = x1 + m_screenWidth + (!x1 ? 0 : m_tileSize);

    y1 = ((cameraPos.second - m_sceneHeight / 2.) - truncf(cameraPos.second - m_sceneHeight / 2.)) * m_tileSize * -1;
    y2 = y1 + m_screenHeight + (!y1 ? 0 : m_tileSize);

    // cout << "scene w h : " << m_sceneWidth << ',' << m_sceneHeight << '\n';
    mx = (int)(cameraPos.first - m_sceneWidth / 2.);
    my = (int)(cameraPos.second - m_sceneHeight / 2.);




    // cout << "mx,my : " << mx << ',' << my << '\n';

    for (float y = y1; y < y2; y += m_tileSize) {
        for (float x = x1; x < x2; x += m_tileSize) {
            int id = map->at(my).at(mx);
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


// normalement inutilise
void Camera::draw(SDL_Renderer *r, vector<SDL_Texture*> *bTextures, vector<vector<int>> *map) {
    SDL_FRect rect = {0, 0, m_tileSize, m_tileSize};
    pair<float, float> cameraPos = m_position.getCoords();

    // w et h locale a la carte
    // float width = 40.;
    // float height = 22.5;


    // coordonees des points de debut et de fin du rectangle sur l'ecran
    float x1 = 0;
    float x2 = m_screenWidth;
    float y1 = 0;
    float y2 = m_screenHeight;

    // coordonees sur la carte
    int mx = 0;
    int my = 0;


    // m_tileSize = WINDOW_WIDTH / width;
    // cout << "tilesize : " << m_tileSize << '\n';
    rect.w = m_tileSize;
    rect.h = m_tileSize;



    x1 = ((cameraPos.first - m_sceneWidth / 2.) - truncf(cameraPos.first - m_sceneWidth / 2.)) * m_tileSize * -1;
    x2 = x1 + m_screenWidth + (!x1 ? 0 : m_tileSize);

    y1 = ((cameraPos.second - m_sceneHeight / 2.) - truncf(cameraPos.second - m_sceneHeight / 2.)) * m_tileSize * -1;
    y2 = y1 + m_screenHeight + (!y1 ? 0 : m_tileSize);

    // cout << "pos cam : " << cameraPos.first << ',' << cameraPos.second << '\n';
    mx = (int)(cameraPos.first - m_sceneWidth / 2.);
    my = (int)(cameraPos.second - m_sceneHeight / 2.);

    for (float y = y1; y < y2; y += m_tileSize) {
        for (float x = x1; x < x2; x += m_tileSize) {
            int id = map->at(my).at(mx);
            rect.x = x;
            rect.y = y;
            if (id > 0) {
                SDL_RenderCopyF(r, bTextures->at(id - 1), NULL, &rect);
            }
            mx++;
        }
        mx = (int)(cameraPos.first - m_sceneWidth / 2.);
        my++;
    }


}


// normalement inutilise
void Camera::drawE(SDL_Renderer *r, std::list<Entity> *elist, std::vector<SDL_Texture*> *texture) {

    SDL_FRect rect; // sur l'ecran
    float width = 40., height = 22.5;
    m_tileSize = WINDOW_WIDTH / width;
    rect.w = m_tileSize;
    rect.h = m_tileSize;
    pair<float, float> ePos; // sur la carte
    SDL_FRect erect; // sur la carte
    pair<float, float> camPos = m_position.getCoords(); // sur la carte



    for (list<Entity>::iterator it = elist->begin(); it != elist->end(); it++) {
        ePos = (*it).getPosition().getCoords();
        erect = (*it).getHitbox();

        rect.x = ((ePos.first - erect.w / 2.) - (camPos.first - width / 2.)) * m_tileSize;
        rect.y = ((ePos.second - erect.h / 2.) - (camPos.second - height / 2.)) * m_tileSize;


        SDL_RenderCopyF(r, texture->at((*it).getTextureId()), NULL, &rect);
    }

}



/*

deplacer checkifvisible vers camera

nettoyage de scene

mode pour suivre la cam par defaut comme avant

bug depassement de map








//////////////////////////////////////////////////////////////////////////////////////

commenter /

voir pour les changements de taille d'ecran /
donc ajuster la taille sur la scene de chaque direction pour garder le ratio tilesize /
-> appeller une fonction qui change la dim /

a suivre : mettre plusieurs cam qui scindent l'ecran ?

si oui alors
mettre un decalage de ou commence la cam
dessiner uniquement dans l'espace de la cam : ne pas deborder



adapter le verrouillage de la cam a lockX et lockY /

pour lock : garder un pointeur sur l'entite et verifier sa position dans la cam /
il faut que la fonction soit appelee tt le temps /

donc camera a sa dim sur la scene et la dim de l'ecran /
cam utilise la carte et dessine les textures correspondantes /
aux bonnes coos de l'ecran sans que scene se soucie de l'ecran de n'importe quelle maniere /

qu'est-ce que tilesize ? une variable qui permet de remplir l'ecran a partir de la camera /

objectif : scene ne se soucie jamais de l'ecran mais que du jeu sans l'affichage /

comment passer de float a int sans cast : /
les coordonnees de la cam sont flottants /
les pixels de l'ecran sont entiers /
voir utilisation de sdl_rendercopyf /

verif hors de carte /

stocker la dim de la cam et la dim finale sur l'ecran /

qui dessine sur l'ecran /
enlever tilesize /

ainsi liberer les textures de la scene /
mais ou les mettre ? /
dans camera ? mais si on a plusieurs cameras ? /
-> charger dynamiquement comme dans scene et si plusieurs cam alors copier le pointeur /
mieux : statique /

*/


