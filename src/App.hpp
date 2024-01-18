
#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>
#include <unordered_map>

#include "Font.hpp"
#include "Scene.hpp"
#include "Camera.hpp"

// ms
#define DEFAULT_DELTA_TIME 16

#define DEFAULT_WINDOW_WIDTH 1280
#define DEFAULT_WINDOW_HEIGHT 720


#define GAME_VERSION "neuro v0.1"







class App {
    public:

    App ();
    ~App ();

    static void init ();

    static void quit ();

    static void initSDL ();

    static void exitSDL ();



    static void main ();

    static void handleEvents ();

    static void doLogic ();

    static void doDisplay ();

    static bool isPressed (SDL_Keycode keycode);



    static SDL_Texture *loadTexture(const char *fileName);

    static SDL_Texture *toTexture (SDL_Surface *surface);


    static void limitFPS ();
    static void limitFPS (unsigned int);


    static int map (int x, int a, int b, int c, int d);

    protected:

    static void initUserInputs ();



    static std::string m_SDLVersion;
    static std::string m_gameVersion;

    static int m_windowWidth;
    static int m_windowHeight;

    static SDL_Window *m_window;
    static SDL_Renderer *m_renderer;

    static int m_deltaTime_ms;

    static bool m_gameLaunched;
    static bool m_gamePaused;

    static std::unordered_map<SDL_Keycode, bool> *m_userInputs;

    // font par defaut
    static Font m_font;

    static Scene m_scene;

    static Camera m_camera;

};



#endif