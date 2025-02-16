#include "graphics/graphic_utils.h"
#include "engine/simulation.h"
#include "engine/system.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_render.h>
#ifdef _WIN32
#include <SDL/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 900
#define VEC_DRAW_SCALE 1000

/**
 * Draws a particle on the screen as a filled circle relative to the center of the window.
 *
 * @param renderer  The SDL renderer context used for drawing
 * @param p The particle to be drawn, containing position and radius
 *
 * @note Position (0,0) will be drawn at the center of the window
 * @note Y-coordinate is inverted (negative y moves up)
 */
void drawParticle(SDL_Renderer *renderer, particle p) {
  int cx = (int)p.pos.x + (int)WINDOW_WIDTH / 2;
  int cy = -(int)p.pos.y + (int)WINDOW_HEIGHT / 2;
  int vx = (int)(p.pos.x + p.vel.x*VEC_DRAW_SCALE) + (int)WINDOW_WIDTH / 2;
  int vy = -(int)(p.pos.y +p.vel.y*VEC_DRAW_SCALE) + (int)WINDOW_HEIGHT / 2;
  circleFill(renderer, (int)p.pos.x + (int)WINDOW_WIDTH / 2,
             -(int)p.pos.y + (int)WINDOW_HEIGHT / 2, p.radius);
  //SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  //SDL_RenderDrawLine(renderer, cx, cy, vx, vy);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

int main(void) {
  SDL_Event event;
  SDL_Renderer *renderer;
  SDL_Window *window;
  int i;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window,
                              &renderer);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

  particle_system system = open_system("./system");

  while (1) {
    // handle quit
    if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
      break;

    // clear screen
    SDL_SetRenderDrawColor(renderer, 199, 198, 185, 0);
    SDL_RenderClear(renderer);

    // draw
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 0; i < system.size; i++)
      drawParticle(renderer, system.particles[i]);
    SDL_RenderPresent(renderer);

    // update logic
    step_system(system, (sim_config){5, 0.000001, 1});
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
