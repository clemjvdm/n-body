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
  circleFill(renderer, (int)p.pos.x + (int)WINDOW_WIDTH / 2,
             -(int)p.pos.y + (int)WINDOW_HEIGHT / 2, p.radius);
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

  particle_system system = createSystem(200, 0.00001);
  // particle_system system = openSystem("system.txt");
  initSystem(system, 800);
    remove_intersections(&system);
    printf("size: %zu\n", system.size);

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
    updateSystem(system, 15, PP);
  }

  saveSystem(system, "system_saved.txt");

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
