#include "meta.h"

namespace meta
{
    int screen_width=0;
    int screen_height=0;
    int view_width=0;
    int view_height=0;
    int map_width=0;
    int map_height=0;
    SDL_Surface * screen=0;
}

//darn, its hard to be lazy about program design, *must resist the urge to make four or more blit functions*
void blit_game(SDL_Surface * image, int x, int y)
{
    SDL_Rect rect;
    SDL_BlitSurface(image, 0, meta::screen, &rect);
}

void blit_screen(SDL_Surface * image, int x, int y)
{

}
