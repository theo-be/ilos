/** 
 * @file main.cpp
 * @version 0.5.1
 * @date 25/08/2022 Debut du projet
 * @brief Fichier principal du projet
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "constants.hpp"




#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <utility> //
#include <algorithm>
#include <chrono>






#include "main.hpp"
#include "Entity.hpp"
#include "Scene.hpp"
#include "Player.hpp"
#include "TextBox.hpp"
#include "Interface.hpp"
#include "texture_management.hpp"
#include "entities.hpp"
#include "Font.hpp"
#include "Vector.hpp"
#include "Camera.hpp"

using namespace std;

int main(int argc, char *argv[]) {

    // version SDL
    SDL_version v;
    SDL_VERSION(&v);

    ostringstream ostr;
    ostr << "SDL v" << (int)v.major << "." << (int)v.minor << "." << (int)v.patch;
    cout << ostr.str() << endl;
    string gameVersion = "ilos v0.5";
    cout << gameVersion << endl;
    
    int exitStatus = EXIT_FAILURE;

    

    /* -------------------------------------------------- */
    // Initialisation de la SDL
    /* -------------------------------------------------- */

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO)) {
        cerr << "ERREUR initialisation SDL : " << SDL_GetError() << endl;
        exit(exitStatus);
    }

    if (!(window = SDL_CreateWindow(gameVersion.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE))) {
        cerr << "ERREUR creation fenetre : " << SDL_GetError() << endl;
        exitSDL(window, renderer);
        exit(exitStatus);
    }

    if (!(renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED))) {
        cerr << "ERREUR creation rendu : " << SDL_GetError() << endl;
        exitSDL(window, renderer);
        exit(exitStatus);
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    if (TTF_Init() < 0) {
        cerr << "ERREUR chargement TTF" << endl;
        exit(exitStatus);
    }

    cout << "SDL chargee" << endl;


    /* -------------------------------------------------- */
    // Debut des initialisations
    // Initialisation de la scene
    /* -------------------------------------------------- */

    cout << "Initialisation de la scene" << endl;

    Scene scene(renderer);
    string mapFileName = "data/maps/map";
    scene.initMap(mapFileName.c_str());
    scene.loadTiles();

    cout << "Scene initialisee" << endl;


    /* -------------------------------------------------- */
    // Initialisation de la camera
    /* -------------------------------------------------- */

    cout << "Initialisation de la camera" << endl;

    Camera camera(0, 0, 40., 22.5, WINDOW_WIDTH, WINDOW_HEIGHT, renderer);


    /* -------------------------------------------------- */
    // Initialisation des textures
    /* -------------------------------------------------- */

    cout << "Chargement des textures des entites" << endl;

    // vector<SDL_Texture*> entityTextures;
    // loadEntityTextures(renderer, entityTextures);

    scene.loadEntityTextures();
    
    camera.loadEntityTextures("ressources/entity");



    cout << "Textures des entites chargees" << endl;

    /* -------------------------------------------------- */
    // Initialisation des textures de la carte
    /* -------------------------------------------------- */


    cout << "Chargement des textures de la carte" << endl;

    camera.loadTilesTextures("ressources/fg");

    cout << "Textures de la carte chargees" << endl;

    cout << "Camera totalement initialisee" << endl;

    
    /* -------------------------------------------------- */
    // Initialisation du joueur
    /* -------------------------------------------------- */

    cout << "Initialisation du joueur" << endl;

    // SDL_Rect prect = {5 * TILE_SIZE, 26 * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    // Vector2D ppos;
    // ppos.setCoords(prect.x + prect.w / 2, prect.y + prect.h / 2);
    // string pname = "Joueur";
    // Player player;

    // scene.getPlayer()->setName(pname);
    // scene.getPlayer()->setTextureId(3);
    // scene.getPlayer()->setHp(10);
    // scene.getPlayer()->setHitbox(prect);
    // scene.getPlayer()->setPosition(ppos);
    // scene.getPlayer()->setPosition0(ppos);

    cout << "Joueur initialise" << endl;

    /* -------------------------------------------------- */
    // Initialisation des entites
    /* -------------------------------------------------- */

    cout << "Initialisation des entites" << endl;


    scene.loadEntities();

    // loadEntities(entityList);
    // showEntityList(entityList);

    showEntityList(scene.getMobList());



    cout << "Entites initialisees" << endl;


    /* -------------------------------------------------- */
    // Initialisation de la police
    /* -------------------------------------------------- */


    cout << "Initialisation de la police" << endl;
    
    // TTF_Font *font = TTF_OpenFont("ressources/fonts/16020_FUTURAM.ttf", FONT_SIZE_INTERFACE);

    Font font("ressources/fonts/16020_FUTURAM.ttf", FONT_SIZE_INTERFACE, renderer);


    
    cout << "Police initialisee" << endl;
    // SDL_Color white;
    // white.r = 255;
    // white.g = 255;
    // white.b = 255;
    // white.a = SDL_ALPHA_OPAQUE;



    /* -------------------------------------------------- */
    // Initialisation des dialogues
    /* -------------------------------------------------- */


    
    


    




    /* -------------------------------------------------- */
    // Autres initialisations
    /* -------------------------------------------------- */

    // SDL_Delay(50);

    bool debug = true;
    
    // utilise pour la taille de la fenetre
    int w, h;

    int up = 0, down = 0, left = 0, right = 0;

    unsigned int frameLimit = 0 + F_DELTA;

    // auto frameDelay = chrono::microseconds(FRAME_DELTA);
    // chrono::steady_clock::time_point timeLimit = chrono::steady_clock::now() + frameDelay;

    unsigned int fpsCounter = 0;

    unsigned int dt = 0;
    // unsigned int lastFrame = SDL_GetTicks();
    // unsigned int secondLastFrame = SDL_GetTicks() - F_DELTA;

    // mesure de temps de debug

    double computationTimeDT;
    chrono::steady_clock::time_point computationTimeBegin = chrono::steady_clock::now();
    chrono::steady_clock::time_point computationTimeEnd = chrono::steady_clock::now();

    double renderingTimeDT;
    chrono::steady_clock::time_point renderingTimeBegin = chrono::steady_clock::now();
    chrono::steady_clock::time_point renderingTimeEnd = chrono::steady_clock::now();


    // 
    double globalDT;
    chrono::steady_clock::time_point frameBegin = chrono::steady_clock::now();
    chrono::steady_clock::time_point frameEnd = chrono::steady_clock::now();

    chrono::duration<double> timeSpanDT;
    chrono::steady_clock::time_point secondLastFrame = chrono::steady_clock::now();
    chrono::steady_clock::time_point lastFrame = chrono::steady_clock::now();


    chrono::duration<double> globalTimeSpan;



    // unsigned int computationTimeDT = 0;
    // unsigned int computationTimeBegin = 0;
    // unsigned int computationTimeEnd = 0;

    // unsigned int renderingTimeDT = 0;
    // unsigned int renderingTimeBegin = 0;
    // unsigned int renderingTimeEnd = 0;

    unsigned int p_dt = 0;

    bool programLaunched = true;
    bool programPaused = false;

    ostringstream toStringConvertor;



    // Initialisation du foncteur d'affichage des textures
    // ShowEntity showEntities(renderer, &scene, scene.getMobEntityTextures());

    cout << "Jeu totalement initialise" << endl;

    SDL_SetRenderDrawColor(renderer, 128, 192, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // scene.moveCamera(*scene.getPlayer()->getTarget());
    // scene.displayScene();

    
    camera.lockTo(*scene.getPlayer()->getTarget());
    camera.displayMap(scene.getMap());
    camera.displayEntities(scene.getMobList());

    // SDL_Delay(500);


    /* -------------------------------------------------- */
    // Debut de la boucle infinie
    /* -------------------------------------------------- */

    while (programLaunched) {


        frameBegin = chrono::steady_clock::now();

        
        dt = SDL_GetTicks() - p_dt;
        p_dt = SDL_GetTicks();

        computationTimeBegin = chrono::steady_clock::now();

        // dt = lastFrame - secondLastFrame;
        // secondLastFrame = lastFrame;
        // lastFrame = SDL_GetTicks();

        fpsCounter = 1000 / dt;

        /* Debut des evenements ------------------------------------------- */

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    programLaunched = false;
                    break;
                /* -------------------------------------------------- */
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_q:
                            left = 1;
                            break;
                        case SDLK_d:
                            right = 1;
                            break;
                        case SDLK_z:
                            up = 1;
                            break;
                        case SDLK_s:
                            down = 1;
                            break;

                        case SDLK_e:
                            // fonction d'interaction avec entite
                            scene.getPlayer()->getTarget()->doInteractions();
                            break;

                        // Debug
                        /* ------------------------------ */


                        case SDLK_p:
                            programLaunched = false;
                            break;
                        case SDLK_KP_MINUS:
                            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
                            break;
                        case SDLK_KP_PLUS:
                            SDL_GetWindowSize(window, &w, &h);
                            camera.setWindowDimension(w, h);
                            break;
                        default:
                            break;
                    }
                    break;


                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        case SDLK_q:
                            left = 0;
                            break;
                        case SDLK_d:
                            right = 0;
                            break;
                        case SDLK_z:
                            up = 0;
                            break;
                        case SDLK_s:
                            down = 0;
                            break;
                        case SDLK_ESCAPE:
                            programPaused = !programPaused;


                        // Debug
                        /* ------------------------------ */




                        case SDLK_b:
                            // printf("interaction : %d\nboite utilisee : %d\n", player.available_interaction, dialogue.used);
                            break;
                        case SDLK_i:
                            // player.playable = player.playable == 1 ? 0 : 1;
                            break;
                        case SDLK_k:
                            break;
                        case SDLK_l:
                            scene.addEntityToPlayerPos();
                            cout << "Nombre d\'entites : " << Entity::getCount() << endl;
                            cout << "Entite cree : " << scene.getMobList()->front().getNamePtr() << endl;
                            showEntityList(scene.getMobList());
                            break;
                        case SDLK_m:
                            // scene.moveCamera(*scene.getPlayer()->getTarget());
                            
                            break;
                        case SDLK_n:
                            SDL_Delay(200);
                            break;
                        case SDLK_o:
                            showEntityList(scene.getMobList());
                            break;
                        case SDLK_v:
                            debug = !debug;
                            cout << "Debug : " << debug << endl;
                        
                        /* ------------------------------ */
                        default:
                            break;
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    break;
                case SDL_MOUSEBUTTONUP:
                    break;

                case SDL_WINDOWEVENT:
                    SDL_GetWindowSize(window, &w, &h);
                    camera.setWindowDimension(w, h);
                    break;
                default:
                    break;
            }
        }
        
        /* Fin des evenements --------------------------------------------- */


        // Programme principal

        if (programPaused) {
            // Afficher le menu de pause
            font.displayText("PAUSE", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, Center);
        }
        else {
            // Executer le jeu
            // Nettoyage de l'arriere plan
            SDL_SetRenderDrawColor(renderer, 128, 192, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);


            /* Actions principales --------------------------------------------- */




            // Actions sur la scene

            // scene.moveCamera(player);
            
            // Actions de joueur

            // player.move(scene, left, right, up, down, dt, renderer);
            

            // player.doActions(entityList, dt);



            scene.doActions(left, right, up, down, dt);          




            // scene.displayScene();

            camera.update();

            camera.displayMap(scene.getMap());
            camera.displayEntities(scene.getMobList());
            

            // Affichage de la carte
            // scene.displayMap();

            // Affichage des entites
            // for_each(entityList.begin(), entityList.end(), showEntities);

            // Affichage du joueur
            // player.show(renderer, scene.getCameraPos(), entityTextures);

            /* Fin des actions principales --------------------------------------------- */

            // font.displayTextWrapped("PogChamp", WINDOW_WIDTH / 2, 500, 200, Center);

            // SDL_Rect fre = {200, 200, 0, 0};
            // SDL_QueryTexture(font.m_atlas, NULL, NULL, &fre.w, &fre.h);
            // SDL_RenderCopy(renderer, font.m_atlas, NULL, &fre);


        }


        /* Debug --------------------------------------------- */

        if (debug) {
            SDL_Point p = {0, 0};
            Entity *target = scene.getPlayer()->getTarget();
            SDL_FRect r = scene.getPlayer()->getTarget()->getHitbox();
            pair<float, float> pos = target->getPosition().getCoords();
            pair<float, float> v = target->getVelocity().getCoords();
            // pair<float, float> v0 = target->getVelocity0().getCoords();
            pair<float, float> a = target->getAcceleration().getCoords();
            pair<int, int> mapDim = scene.getMapDim();
            // SDL_Rect camera = scene.getCameraPos();
            pair<float,float> campos = scene.getCamera()->getPosition().getCoords();


            // INFORMATIONS SUR LE MONDE
            SDL_Rect bg = {0, 0, 300, 160};
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(renderer, &bg);

            // IPS
            toStringConvertor.str("");
            toStringConvertor << fpsCounter << "IPS";
            font.displayText(toStringConvertor.str().c_str(), p.x, p.y);

            // NOMBRE D'ENTITES
            toStringConvertor.str("");
            toStringConvertor << scene.getMobList()->size() << " entités chargées";
            p.y += FONT_SIZE_INTERFACE;
            font.displayText(toStringConvertor.str().c_str(), p.x, p.y);
            // DT
            toStringConvertor.str("");
            toStringConvertor << "dt : " << globalDT * 1000 << "ms";
            p.y += FONT_SIZE_INTERFACE;
            font.displayText(toStringConvertor.str().c_str(), p.x, p.y);
            // TAILLE DE CARTE
            toStringConvertor.str("");
            toStringConvertor << "taille carte : " << mapDim.first << " x " << mapDim.second;
            p.y += FONT_SIZE_INTERFACE;
            font.displayText(toStringConvertor.str().c_str(), p.x, p.y);
            // TAILLE DE TUILE
            toStringConvertor.str("");
            toStringConvertor << "taille tuile : " << scene.getTileSize() << "px";
            p.y += FONT_SIZE_INTERFACE;
            font.displayText(toStringConvertor.str().c_str(), p.x, p.y);
            // TEMPS DE CALCUL
            toStringConvertor.str("");
            toStringConvertor << "temps de calcul : " << computationTimeDT * 1000 << "ms";
            p.y += FONT_SIZE_INTERFACE;
            font.displayText(toStringConvertor.str().c_str(), p.x, p.y);
            // TEMPS DE RENDU
            toStringConvertor.str("");
            toStringConvertor << "temps de rendu : " << renderingTimeDT * 1000 << "ms";
            p.y += FONT_SIZE_INTERFACE;
            font.displayText(toStringConvertor.str().c_str(), p.x, p.y);
            // TEMPS DE RENDU
            toStringConvertor.str("");
            toStringConvertor << "temps total : " << (renderingTimeDT + computationTimeDT) * 1000 << "ms";
            p.y += FONT_SIZE_INTERFACE;
            font.displayText(toStringConvertor.str().c_str(), p.x, p.y);
            // POSITION DE LA CAM
            toStringConvertor.str("");
            toStringConvertor << "Position de la caméra : " << campos.first << "x" << campos.second << "y";
            p.y += FONT_SIZE_INTERFACE;
            font.displayText(toStringConvertor.str().c_str(), p.x, p.y);

            bg.w = 260;
            bg.x = WINDOW_WIDTH - bg.w;
            bg.h = 260;
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(renderer, &bg);
            
            // INFORMATIONS DU JOUEUR
            p.x = WINDOW_WIDTH - 1;
            p.y = 0;
            // TITRE
            toStringConvertor.str("");
            toStringConvertor << "JOUEUR";
            font.displayText(toStringConvertor.str().c_str(), p.x, p.y, Right);
            // PV
            toStringConvertor.str("");
            toStringConvertor << "PV : " << target->getHp();
            p.y += FONT_SIZE_INTERFACE;
            font.displayText(toStringConvertor.str().c_str(), p.x, p.y, Right);
            // HITBOX
            toStringConvertor.str("");
            toStringConvertor << "Hitbox : " << r.w << " x " << r.h;
            p.y += FONT_SIZE_INTERFACE;
            font.displayText(toStringConvertor.str().c_str(), p.x, p.y, Right);
            // TOUCHE LE SOL
            toStringConvertor.str("");
            toStringConvertor << "Touche le sol : " << target->doTouchGround();
            p.y += FONT_SIZE_INTERFACE;
            font.displayText(toStringConvertor.str().c_str(), p.x, p.y, Right);
            // TEMPS DE VOL
            toStringConvertor.str("");
            toStringConvertor << "Temps de vol (ms) : " << target->getFlightTime();
            p.y += FONT_SIZE_INTERFACE;
            font.displayText(toStringConvertor.str().c_str(), p.x, p.y, Right);
            // TEMPS D'INVINCIBILITE
            toStringConvertor.str("");
            toStringConvertor << "Temps d'invincibilite (ms) : " << target->getInvincibilityTime();
            p.y += FONT_SIZE_INTERFACE;
            font.displayText(toStringConvertor.str().c_str(), p.x, p.y, Right);
            // INTERACTION
            toStringConvertor.str("");
            toStringConvertor << "Interaction : " << target->hasAvailableInteraction();
            p.y += FONT_SIZE_INTERFACE;
            font.displayText(toStringConvertor.str().c_str(), p.x, p.y, Right);
            // POSITION
            toStringConvertor.str("");
            toStringConvertor << "Position : " << pos.first << "x " << pos.second << "y";
            p.y += FONT_SIZE_INTERFACE;
            font.displayText(toStringConvertor.str().c_str(), p.x, p.y, Right);
            // HITBOX
            toStringConvertor.str("");
            toStringConvertor << "Boite de collision : " << r.x << "x " << r.y << "y";
            p.y += FONT_SIZE_INTERFACE;
            font.displayText(toStringConvertor.str().c_str(), p.x, p.y, Right);
            // VITESSE
            toStringConvertor.str("");
            toStringConvertor << "Vitesse : " << v.first << "x " << v.second << "y";
            p.y += FONT_SIZE_INTERFACE;
            font.displayText(toStringConvertor.str().c_str(), p.x, p.y, Right);
            // // VITESSE 0
            // toStringConvertor.str("");
            // toStringConvertor << "Vitesse0 : " << v0.first << "x " << v0.second << "y";
            // p.y += FONT_SIZE_INTERFACE;
            // font.displayText(toStringConvertor.str().c_str(), p.x, p.y, Right);
            // ACCELERATION
            toStringConvertor.str("");
            toStringConvertor << "Acceleration : " << a.first << "x " << a.second << "y";
            p.y += FONT_SIZE_INTERFACE;
            font.displayText(toStringConvertor.str().c_str(), p.x, p.y, Right);
            // ID DE TEXTURE
            toStringConvertor.str("");
            toStringConvertor << "Id texture : " << target->getTextureId();
            p.y += FONT_SIZE_INTERFACE;
            font.displayText(toStringConvertor.str().c_str(), p.x, p.y, Right);
            // ENTITE CIBLE INTERACTION
            toStringConvertor.str("");
            toStringConvertor << "Entite d\'interaction : ";
            if (target->hasAvailableInteraction()) toStringConvertor << target->getInteractionEntityTarget()->getNamePtr();
            else toStringConvertor << "NULL";
            p.y += FONT_SIZE_INTERFACE;
            font.displayText(toStringConvertor.str().c_str(), p.x, p.y, Right);


        }


        computationTimeEnd = chrono::steady_clock::now();

        globalTimeSpan = chrono::duration_cast<chrono::duration<double>>(computationTimeEnd - computationTimeBegin);

        computationTimeDT = globalTimeSpan.count();

        // computationTimeDT = chrono::duration_cast<chrono::duration<double>>(computationTimeEnd - computationTimeBegin).count();
        renderingTimeBegin = chrono::steady_clock::now();


        SDL_RenderPresent(renderer);

        renderingTimeEnd = chrono::steady_clock::now();
        renderingTimeDT = chrono::duration_cast<chrono::duration<double>>(renderingTimeEnd - renderingTimeBegin).count();


        // Limite d'images par secondes
        limitFPS(frameLimit);
        frameLimit = SDL_GetTicks() + F_DELTA;

        // sleepUntil(timeLimit);
        // timeLimit = chrono::steady_clock::now() + frameDelay;



        frameEnd = chrono::steady_clock::now();
        globalDT = chrono::duration_cast<chrono::duration<double>>(frameEnd - frameBegin).count();

        timeSpanDT = chrono::duration_cast<chrono::duration<double>>(lastFrame - secondLastFrame);
        globalDT = timeSpanDT.count();

        secondLastFrame = lastFrame;
        lastFrame = frameEnd;



    }
    













    /* -------------------------------------------------- */
    // Fin du programme
    /* -------------------------------------------------- */

    cout << "Dechargement des entites" << endl;

    // entityList.clear();

    scene.unloadEntities();

    cout << "Entites dechargees" << endl;



    cout << "Dechargement de la carte" << endl;

    scene.unloadMap();

    cout << "Carte dechargee" << endl;

    cout << "Dechargement des textures" << endl;

    scene.unloadTiles();
    camera.unloadTilesTextures();

    cout << "Tuiles dechargees" << endl;

    scene.unloadEntityTextures();
    camera.unloadEntityTextures();
    // unloadEntityTextures(entityTextures);

    cout << "Textures des entites dechargees" << endl;

    cout << "Textures dechargees" << endl;

    cout << "Dechargement de la police" << endl;

    // TTF_CloseFont(font);
    font.clear();

    cout << "Police dechargee" << endl;


    exitSDL(window, renderer);
    cout << "SDL dechargee, TTF decharge" << endl;

    // system("pause");

    exitStatus = EXIT_SUCCESS;
    return exitStatus;
}



/**
 * @fn void exitSDL (SDL_Window *w, SDL_Renderer *r)
 * @param r rendu
 * @param w fenetre
 * @brief Detruit le rendu et la fenetre et quitte SDL
 * @date 25/08/2022
*/
void exitSDL (SDL_Window *w, SDL_Renderer *r) {
    if (r) SDL_DestroyRenderer(r);
    if (w) SDL_DestroyWindow(w);
    TTF_Quit();
    SDL_Quit();
}


/**
 * @fn void limitFPS (unsigned int limit)
 * @param limit limite
 * @brief Limite l'execution du programme pour avoir un taux d'images par seconde stable
 * @date 25/08/2022
*/
void limitFPS (unsigned int limit) {
    unsigned int ticks = SDL_GetTicks();

    if (limit < ticks) return;
    else if (limit > ticks + F_DELTA) SDL_Delay(F_DELTA);
    else SDL_Delay(limit - ticks);

    
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


/**
 * 
*/
void initFont () {
}

/**
 * 
*/
void executeCoreGame () {
    
}

/**
 * 
*/
void displayPauseMenu () {

}
