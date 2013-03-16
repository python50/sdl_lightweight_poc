#ifndef META_H_INCLUDED
#define META_H_INCLUDED

#include "SDL.h"

namespace meta
{
    extern int screen_width;
    extern int screen_height;
    extern int view_width;
    extern int view_height;
    extern int map_width;
    extern int map_height;
    extern SDL_Surface * screen;
}

void blit(SDL_Surface * image, int x, int y);
void blit_screen(SDL_Surface * image, int x, int y);



#endif // META_H_INCLUDED
