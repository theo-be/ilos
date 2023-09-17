
#ifndef MAIN_H
#define MAIN_H

#include <SDL.h>

#include <chrono>




void exitSDL(SDL_Window *w, SDL_Renderer *r);


void limitFPS (unsigned int limit);

void sleepUntil (const std::chrono::steady_clock::time_point &targetTime);

void initFont ();

void executeCoreGame ();

void displayPauseMenu ();


#endif







