#ifndef GRAPHIC_UTILS_H
#define GRAPHIC_UTILS_H

#include "../engine/nbody.h"
#include <SDL2/SDL_render.h>

#ifdef _WIN32
#include <SDL/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

void circle(SDL_Renderer *renderer, int center_x, int center_y, int radius);
void circleFill(SDL_Renderer *renderer, int center_x, int center_y, int radius);

#endif
