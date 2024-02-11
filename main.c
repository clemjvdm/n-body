#include "graphics/graphic_utils.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_render.h>
#ifdef _WIN32
#include <SDL/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include "engine/nbody.h"
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

void drawParticle(SDL_Renderer *renderer, particle p) {
  circleFill(renderer, (int)p.pos.x + (int)WINDOW_WIDTH / 2,
             (int)p.pos.y + (int)WINDOW_HEIGHT / 2, p.radius);
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

  particle_system system =
      createSystem(100, 0.00001); // openSystem("system.txt");
  initSystem(system, 800);
  // saveSystem(system, "system1.txt");

  for (int i = 0; i < 4; i++) {
    particle p = system.particles[i];
    printf("p%d\nmass: %lf\nradius: %lf\npos.x: %lf\npos.y: %lf\n", i, p.mass,
           p.radius, p.pos.x, p.pos.y);
  }
  while (1) {
    // handle quit
    if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
      break;

    // clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // draw
    SDL_SetRenderDrawColor(renderer, 255, 20, 147, 255);
    for (int i = 0; i < system.size; i++)
      drawParticle(renderer, system.particles[i]);
    SDL_RenderPresent(renderer);

    // update logic
    updateSystem(system, 5, PP);
  }

  saveSystem(system, "system_saved.txt");

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
