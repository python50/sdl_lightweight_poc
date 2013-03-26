#ifndef META_H_INCLUDED
#define META_H_INCLUDED

#include "Box2D.h"
#include "SDL.h"
#include "base.h"
#include <vector>
#include <utility>
namespace meta
{
extern int screen_width;
extern int screen_height;
extern int view_x;
extern int view_y;
extern int map_width;
extern int map_height;
extern SDL_Surface * screen;
extern b2Vec2 gravity;
extern b2World world;

// vector   -> pair		->  id
//          			->  surface*
extern std::vector<std::pair<std::string , SDL_Surface *> > surfaces;
// vector	-> base*
extern std::vector<base *> objects;

extern char background_red;
extern char background_green;
extern char background_blue;
}

SDL_Surface * load_surface(std::string filename, char mode);
void add_surface(std::string id, SDL_Surface * surface);
SDL_Surface * get_surface(std::string id);

bool collide_rect(SDL_Rect a , SDL_Rect b);
void blit_game(SDL_Surface * image, int x, int y);
void blit_screen(SDL_Surface * image, int x, int y);

std::vector<std::pair<int, int> > b2poly_convert(b2PolygonShape * shape);
char * itoa(long val, char * buf);
bool remove_object(void * ptr);


void set_background(int red, int green, int blue);

#endif // META_H_INCLUDED
