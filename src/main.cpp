/** 
 * @file main.cpp
 * @version 0.5.1
 * @date 25/08/2022 Debut du projet
 * @brief Fichier principal du projet
*/


#include "main.hpp"
#include "App.hpp"

int main(int argc, char *argv[]) {

    App::init();

    App::main();


    App::quit();

    return 0;
}
