

#include <SDL2/SDL.h>

#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <chrono>
#include <unordered_map>
#include <iterator>
#include <algorithm>
#include <utility>


#include "App.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Font.hpp"
#include "Debug.hpp"
#include "Entity.hpp"
#include "Vector.hpp"
#include "Player.hpp"


using namespace std;


string App::m_gameVersion = "";
string App::m_SDLVersion = "";
int App::m_windowWidth = 0;
int App::m_windowHeight = 0;
SDL_Window *App::m_window = nullptr;
SDL_Renderer *App::m_renderer = nullptr;
float App::m_deltaTime_ms = DEFAULT_DELTA_TIME;
bool App::m_gameLaunched = true;
bool App::m_gamePaused = false;
unordered_map<SDL_Keycode, bool> *App::m_userInputs = nullptr;
Font *App::m_font = nullptr;
Scene *App::m_scene = nullptr;
Camera *App::m_camera = nullptr;


App::App ()
{}

App::~App () {
}

void App::init () {
    SDL_version v;
    SDL_VERSION(&v);
    m_SDLVersion.clear();
    ostringstream convertor;
    convertor.str("");
    convertor << "SDL v" << (int)v.major << '.' << (int)v.minor << '.' << (int)v.patch;
    m_SDLVersion = convertor.str();
    m_gameVersion = GAME_VERSION;
    
    m_windowWidth = DEFAULT_WINDOW_WIDTH;
    m_windowHeight = DEFAULT_WINDOW_HEIGHT;


    /* -------------------------------------------------- */
    // Initialisation de SDL
    /* -------------------------------------------------- */

    initSDL();

    cout << "SDL chargee" << endl;

    // scene

    // charger les entites et le joueur

    cout << "Initialisation de la scene" << endl;

    m_scene = new Scene();

    m_scene->init();

    
    cout << "Initialisation des entites" << endl;
    cout << "Entites initialisees" << endl;

    cout << "Initialisation du joueur" << endl;
    cout << "Joueur initialise" << endl;

    cout << "Scene initialisee" << endl;


    // camera

    cout << "Initialisation de la camera" << endl;

    m_camera = new Camera(DEFAULT_SCENE_WIDTH, DEFAULT_SCENE_HEIGHT, m_windowWidth, m_windowHeight, m_renderer);
    
    cout << "Chargement des textures des entites" << endl;

    Camera::loadEntityTextures(DEFAULT_ENTITY_LOCATION);
    cout << "Textures des entites chargees" << endl;

    Camera::loadTilesTextures(DEFAULT_TILE_LOCATION);

    
    cout << "Textures de la carte chargees" << endl;

    m_camera->setMode(TargetEntity);
    m_camera->setTarget(m_scene->getPlayer()->getTarget());
    m_camera->setTargetScreenArea(.67, .5);

    cout << "Camera totalement initialisee" << endl;
    // police

    cout << "Initialisation de la police" << endl;

    m_font = new Font(DEFAULT_FONT_LOCATION, DEFAULT_FONT_SIZE, m_renderer);

    cout << "Police initialisee" << endl;


    // autre

    m_userInputs = new unordered_map<SDL_Keycode, bool>;
    initUserInputs();

    cout << "Jeu totalement initialise" << endl;
    


}




void App::quit () {

    cout << "Dechargement des entrees utilisateur" << endl;

    m_userInputs->clear();
    delete m_userInputs;

    cout << "Entrees utilisateur dechargees" << endl;

    cout << "Dechargement des entites" << endl;

    m_scene->unloadEntities();
    

    cout << "Entites dechargees" << endl;

    cout << "Dechargement de la carte" << endl;
    m_scene->unloadMap();

    cout << "Carte dechargee" << endl;

    delete m_scene;

    cout << "Dechargement des textures" << endl;

    Camera::unloadTilesTextures();
    cout << "Tuiles dechargees" << endl;
    Camera::unloadEntityTextures();
    cout << "Textures des entites dechargees" << endl;

    

    delete m_camera;

    cout << "Textures dechargees" << endl;

    cout << "Dechargement de la police" << endl;
    m_font->clear();
    delete m_font;
    cout << "Police dechargee" << endl;

    exitSDL();
    cout << "SDL dechargee, TTF decharge" << endl;


}

void App::initSDL () {

    cout << m_gameVersion << '\n';
    cout << m_SDLVersion << '\n';

    int SDLInitialized = SDL_Init(SDL_INIT_EVERYTHING);
    if (SDLInitialized) {
        cerr << "ERREUR initialisation SDL : " << SDL_GetError() << '\n';
        exit(-1);
    }

    m_window = SDL_CreateWindow(m_gameVersion.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_windowWidth, m_windowHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

    if (!m_window) {
        cerr << "ERREUR creation fenetre : " << SDL_GetError() << '\n';
        exitSDL();
        exit(-1);
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer) {
        cerr << "ERREUR creation rendu : " << SDL_GetError() << '\n';
        exitSDL();
        exit(-1);
    }

    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

    int TTFInitialized = TTF_Init();

    if (TTFInitialized) {
        cerr << "ERREUR initialisation TTF\n";
        exitSDL();
        exit(-1);
    }
}


void App::exitSDL () {
    if (m_renderer) 
        SDL_DestroyRenderer(m_renderer);

    if (m_window)
        SDL_DestroyWindow(m_window);
    TTF_Quit();
    SDL_Quit();
}

void App::initUserInputs () {
    
    m_userInputs->clear();


    // lettres

    SDL_Keycode key;

    for (key = SDLK_a; key <= SDLK_z; key++) {
        m_userInputs->insert_or_assign(key, false);
    }

    // nombres



    for (key = SDLK_0; key <= SDLK_9; key++) {
        m_userInputs->insert_or_assign(key, false);
    }

    m_userInputs->insert_or_assign(SDLK_LSHIFT, false);
    m_userInputs->insert_or_assign(SDLK_RSHIFT, false);
    m_userInputs->insert_or_assign(SDLK_LCTRL, false);
    m_userInputs->insert_or_assign(SDLK_RCTRL, false);
    m_userInputs->insert_or_assign(SDLK_SPACE, false);
    m_userInputs->insert_or_assign(SDLK_LALT, false);
    m_userInputs->insert_or_assign(SDLK_RALT, false);
    m_userInputs->insert_or_assign(SDLK_COMMA, false);
    m_userInputs->insert_or_assign(SDLK_EXCLAIM, false);
    m_userInputs->insert_or_assign(SDLK_COLON, false);
    m_userInputs->insert_or_assign(SDLK_SEMICOLON, false);
    m_userInputs->insert_or_assign(SDLK_ESCAPE, false);
}




void App::main () {




    while (m_gameLaunched) {
        // evenements hors du jeu
        handleEvents();

        if (!m_gamePaused)
            doLogic();

        doDisplay();

        // limitFPS(SDL_GetTicks() - 16);

        SDL_Delay(DEFAULT_DELTA_TIME);
    }


}


void App::handleEvents () {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                m_gameLaunched = false;
                break;
            case SDL_KEYDOWN:

                m_userInputs->insert_or_assign(event.key.keysym.sym, true);
                
                break;

            case SDL_KEYUP:
                m_userInputs->insert_or_assign(event.key.keysym.sym, false);
                break;

            case SDL_WINDOWEVENT:
                getWindowDimension();
                break;

            
            default:
                break;
        }

        Debug::update();
    }



}

void App::doLogic () {

    m_scene->update();
    m_camera->update();

}

void App::doDisplay () {

    SDL_SetRenderDrawColor(m_renderer, 128, 192, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_renderer);



    m_camera->displayMap(m_scene->getMap());
    m_camera->displayEntities(m_scene->getMobList());




    SDL_RenderPresent(m_renderer);
}
























bool App::isPressed (SDL_Keycode keycode) {
    return m_userInputs->at(keycode);
}

float App::getDeltaTime () {
    return m_deltaTime_ms;
}

/**
 * @fn SDL_Texture *App::loadTexture (const string &fileName)
 * @param fileName nom du fichier source
 * @returns la texture
 * @brief charge une texture
*/
SDL_Texture *App::loadTexture (const char *fileName) {
    SDL_Surface *surface;

    surface = SDL_LoadBMP(fileName);
    if (!surface) {
        cerr << "Erreur de chargement de fichier : " << fileName << ", " << SDL_GetError() << endl;
        return nullptr;
    }
    return toTexture(surface);
}

/**
 * @fn SDL_Texture *App::toTexture (SDL_Surface *surface)
 * @param surface surface
 * @returns la texture
 * @brief transforme une surface en texture et libere la surface
*/
SDL_Texture *App::toTexture (SDL_Surface *surface) {
    SDL_Texture *texture;
    texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        cerr << "Erreur de creation de texture, " << SDL_GetError() << endl;
        exit(-2);
        return nullptr;
    }
    return texture;
}


/**
 * @fn void App::limitFPS (unsigned int limit)
 * @param limit limite
 * @brief Limite l'execution du programme pour avoir un taux d'images par seconde stable
*/
void App::limitFPS (unsigned int limit) {
    unsigned int ticks = SDL_GetTicks();

    if (limit < ticks) return;
    else if (limit > ticks + DEFAULT_DELTA_TIME) SDL_Delay(DEFAULT_DELTA_TIME);
    else SDL_Delay(limit - ticks);

    
}

int App::map (int x, int a, int b, int c, int d) {
    return (x - a) * ((d - c) / (b - a)) + c;
}

void App::getWindowDimension () {
    SDL_GetWindowSize(m_window, &m_windowWidth, &m_windowHeight);

    m_camera->setWindowDimension(m_windowWidth, m_windowHeight);
}
