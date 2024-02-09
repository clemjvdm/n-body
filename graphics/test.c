#include <SDL2/SDL_error.h>
#include <SDL2/SDL_render.h>
#ifdef _WIN32
#include <SDL/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// TODO: write report on this function because its cool and i'd rather not
// forget
// TODO: this function can be further optimized by making single api call using
// SLD_RenderDrawPoints
void circle(SDL_Renderer *renderer, int center_x, int center_y, int radius) {
  int x = radius - 1;
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

    y++;
    err += dy;
    dy += 2;

    if (err > 0) {
      x--;
      err += dx;
      dx += 2;
    }
  }
}

void hline(SDL_Renderer *renderer, int x1, int x2, int y) {
  for (int i = x1; i <= x2; i++)
    SDL_RenderDrawPoint(renderer, i, y);
}

void circleFill(SDL_Renderer *renderer, int center_x, int center_y,
                int radius) {
  int x = radius - 1;
  int y = 0;
  int dx = 1 - (x << 1);
  int dy = 1;
  int err = (dy << 1) + dx;

  while (x >= y) {
    SDL_RenderDrawLine(renderer, center_x - x, center_y - y, center_x + x,
                       center_y - y);
    SDL_RenderDrawLine(renderer, center_x + y, center_y - x, center_x - y,
                       center_y - x);
    SDL_RenderDrawLine(renderer, center_x - x, center_y + y, center_x + x,
                       center_y + y);
    SDL_RenderDrawLine(renderer, center_x - y, center_y + x, center_x + y,
                       center_y + x);

    y++;
    err += dy;
    dy += 2;

    if (err > 0) {
      x--;
      err += dx;
      dx += 2;
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
  circleFill(renderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 200);
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
