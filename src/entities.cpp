#include <SDL2/SDL.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <deque>

#include "entities.hpp"
#include "Entity.hpp"
#include "texture_management.hpp"
#include "Player.hpp"


using namespace std;


/**
 * @fn void showEntityList (list<Entity> &list)
 * @param list liste des entites
 * @brief affiche dans la console la liste des entites
*/
void showEntityList (list<Entity> *list) {
    cout << "-----------------" << endl << Entity::getCount() << "entites" << endl;

    for (std::list<Entity>::iterator it = list->begin(); it != list->end(); it++) {
        cout << (*it).getName() << ", ";
    }
    cout << endl << "-----------------" << endl;
}

/**
 * @fn void killEntity (list<Entity> &list, Entity &e)
 * @param list liste des entites
 * @param e entiter a tuer
 * @brief retire l'entite de la liste
*/
void killEntity (list<Entity> &list, Entity &e) {
    list.remove(e);
}

/**
 * @fn int loadEntityTextures (SDL_Renderer *renderer, deque<SDL_Texture*> &entityTextures)
 * @param renderer renderer
 * @param entityTextures liste des textures
 * @returns 0 en cas de reussite
 * @brief charge les textures des entites dans la liste des textures des entites
*/
int loadEntityTextures (SDL_Renderer *renderer, deque<SDL_Texture*> &entityTextures) {
    string fileName = "";
    ostringstream stream;
    SDL_Texture *texture = NULL;
    int i = 0;

    while (1) {
        i++;
        stream.str("");
        stream << "ressources/entity" << i << ".bmp";
        fileName = stream.str();
        texture = loadTexture(renderer, fileName);
        if (texture) entityTextures.push_back(texture);
        else break;
    }
    return 0;
}


/**
 * @fn void unloadEntityTextures (deque<SDL_Texture*> &entityTextures)
 * @param entityTextures liste des textures des entites
 * @brief decharge les textures des entites de la liste
*/
void unloadEntityTextures (deque<SDL_Texture*> &entityTextures) {
    for (int i = 0; i < ENTITY_TEXTURE_COUNT; i++) {
        SDL_DestroyTexture(entityTextures.at(i));
    }
    entityTextures.clear();
}


/**
 * @fn void addEntityToPlayerPos (list<Entity> &entityList, Player &player)
 * @param entityList liste des entites
 * @param player joueur
 * @brief ajoute une entite a la position du joueur
 * @see Scene::addEntityToPlayerPos
*/
void addEntityToPlayerPos (list<Entity> &entityList, Player &player) {
    Entity e;
    ostringstream stream;
    stream << "PEDRO" << e.getId();
    string name = stream.str();
    SDL_FRect h = player.getTarget()->getHitbox();
    e.setName(name);
    e.setHitbox(h);
    e.setTextureId(0);
    e.setPassive(false);
    entityList.push_front(e);
    // On incremente car l'entite va etre detruite a la fin du bloc
    Entity::increaseCount();
}

/**
 * @fn int loadEntities (list<Entity> &entityList)
 * @param entityList liste des entites
 * @returns 0 en cas de succes
 * @brief charge les entites du fichier de sauvegarde
 * @remarks inutilise, voir Scene::loadEntities
 * @see Scene::loadEntities
*/
int loadEntities (list<Entity> &entityList) {
    ifstream file;
    Entity e;
    SDL_FRect h = {0, 0, TILE_SIZE, TILE_SIZE};
    string fileName = "data/entities/entities";
    // stringstream stream;
    file.open(fileName);
    if (!file) cerr << "ERREUR ouverture " << fileName << endl;
    string entityName = "";
    int value;
    // bool read = true;
    // string s;
    

    // NOM X Y PV PASSIVE TID DID
    while (!file.fail()) {
        // getline(file, s);
        // stream.clear();
        // stream << s;
        // if (stream.str() == "") {
        //     read = false;
        // }
        // cout << stream.str();
        file >> entityName;
        e.setName(entityName);
        file >> value;
        h.x = value * TILE_SIZE;
        file >> value;
        h.y = value * TILE_SIZE;
        e.setHitbox(h);
        file >> value;
        e.setHp(value);
        file >> value;
        e.setPassive(value);
        file >> value;
        e.setTextureId(value);
        entityList.push_front(e);
        Entity::increaseCount();
    }

    file.close();
    return 0;
}

/**
 * @fn bool checkEntityCollision (list<Entity> &list, Entity &target)
 * @param list liste des entites
 * @param target
*/
bool checkEntityCollision (list<Entity> &list, Entity &target) {
    bool touch = false;
    target.clearEntityColliding();
    for (std::list<Entity>::iterator it = list.begin(); it != list.end(); it++) {
        if (target.collideWith(*it)) {
            // cout << "touche" << endl;
            // cout << (*it).getName() << endl;
            target.addEntityColliding(&(*it));
            touch = true;
        }
    }
    return touch;
}
