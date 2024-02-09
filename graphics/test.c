#include <SDL2/SDL_error.h>
#include <SDL2/SDL_render.h>
#include <stdio.h>
#ifdef _WIN32
#include <SDL/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// TODO: this function is off by 1 px because x == radius and we draw at
// center_x + x. Fix!
// TODO: write report on this function because its cool and i'd rather not
// forget
// TODO: this function can be further optimized by making single api call using
// SLD_RenderDrawPoints
void circle(SDL_Renderer *renderer, int center_x, int center_y, int radius) {
  fprintf(stderr, "attempting to print circle!\n");
  int x = radius;
  int y = 0;
  int dx = 1 - (x << 1);
  int dy = 1;
  int err = (dy << 1) + dx;

  while (x >= y) {
    SDL_RenderDrawPoint(renderer, center_x + x, center_y - y);
    SDL_RenderDrawPoint(renderer, center_x + x, center_y + y);
    SDL_RenderDrawPoint(renderer, center_x - x, center_y - y);
    SDL_RenderDrawPoint(renderer, center_x - x, center_y + y);
    SDL_RenderDrawPoint(renderer, center_x + y, center_y - x);
    SDL_RenderDrawPoint(renderer, center_x + y, center_y + x);
    SDL_RenderDrawPoint(renderer, center_x - y, center_y - x);
    SDL_RenderDrawPoint(renderer, center_x - y, center_y + x);

    if (err <= 0) {
      y++;
      err += dy;
      dy += 2;
    }

    if (err > 0) {
      y++;
      x--;
      err += dy + dx;
      dx += 2;
      dy += 2;
    }
  }
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
  circle(renderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 200);
  for (i = 0; i < WINDOW_WIDTH; ++i)
    SDL_RenderDrawPoint(renderer, i, i);
  SDL_RenderPresent(renderer);
  while (1) {
    if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
      break;
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
