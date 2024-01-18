

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


#include "App.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Font.hpp"
#include "Debug.hpp"


using namespace std;


string App::m_gameVersion = "";
string App::m_SDLVersion = "";
int App::m_windowWidth = 0;
int App::m_windowHeight = 0;
SDL_Window *App::m_window = nullptr;
SDL_Renderer *App::m_renderer = nullptr;
int App::m_deltaTime_ms = 0;
bool App::m_gameLaunched = true;
unordered_map<SDL_Keycode, bool> *App::m_userInputs = nullptr;
Font App::m_font(DEFAULT_FONT_LOCATION, DEFAULT_FONT_SIZE, App::m_renderer);
Scene App::m_scene;
Camera App::m_camera;


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

    initSDL();

    
    m_font.setRenderer(m_renderer);
    m_font.init(DEFAULT_FONT_LOCATION, DEFAULT_FONT_SIZE);

    m_camera.setRenderer(m_renderer);
    m_camera.setWindowDimension(m_windowWidth, m_windowHeight);
    m_camera.setSceneDimension(1280, 720);

    m_userInputs = new unordered_map<SDL_Keycode, bool>;
    initUserInputs();

    


}




void App::quit () {



    m_userInputs->clear();
    delete m_userInputs;

    exitSDL();
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

    cout << "SDL initialisee\n";


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

        doLogic();

        doDisplay();

        // limitFPS(SDL_GetTicks() - 16);

        SDL_Delay(DEFAULT_DELTA_TIME);
    }


    // SDL_Delay(3000);

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

            
            default:
                break;
        }

        Debug::update();
    }



}

void App::doLogic () {
    // m_scene.update();
}

void App::doDisplay () {

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_renderer);


    // m_camera.displayEntities(m_scene.getEntityList());

    SDL_Color c = {255, 255, 255, SDL_ALPHA_OPAQUE};
    m_font.displayText("Yo la street", 0, 0, Left, &c);


    SDL_RenderPresent(m_renderer);
}
























bool App::isPressed (SDL_Keycode keycode) {
    return m_userInputs->at(keycode);
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