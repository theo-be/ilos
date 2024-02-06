/** 
 * @file main.cpp
 * @version 0.5.1
 * @date 25/08/2022 Debut du projet
 * @brief Fichier principal du projet
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


#include "main.hpp"
#include "App.hpp"

using namespace std;

int main(int argc, char *argv[]) {

    App::init();

    App::main();


    App::quit();

    return 0;
}


/**
 * @fn void sleepUntil (const chrono::steady_clock::time_point &targetTime)
 * @param targetTime point temporel en microsecondes
 * @brief Met en pause l'execution du programme afin d'avoir de taux de rafraichissement 
*/
void sleepUntil (const chrono::steady_clock::time_point &targetTime) {
    chrono::steady_clock::time_point now = chrono::steady_clock::now();
    for (;;) {
        now = chrono::steady_clock::now();
        if (chrono::duration_cast<chrono::duration<double>>(targetTime - now).count() <= .0) return;
    }
}
