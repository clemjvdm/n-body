#include "graphic_utils.h"

// TODO: write report on this function because its cool and i'd rather not
// forget
// TODO: this function can be further optimized by making single api call using
// SLD_RenderDrawPoints
/**
 * Draw a circle to an SDL renderer. 
 *
 * @param renderer SDL renderer
 * @param center_x center x coordinate of cirlce
 * @param center_y center y coordinate of circle
 * @param radius  radius of circle
 */
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

/**
 * Draw a filled circle to an SDL renderer. 
 *
 * @param renderer SDL renderer
 * @param center_x center x coordinate of cirlce
 * @param center_y center y coordinate of circle
 * @param radius  radius of circle
 */
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
