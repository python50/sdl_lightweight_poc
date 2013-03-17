#ifndef GAME_POLY_H
#define GAME_POLY_H
#include <vector>
#include "SDL.h"
#include "base.h"
#include "SDL_gfxPrimitives.h"


//a STATIC polygon, not moving
class game_poly : public base
{
public:
    int x;
    int y;
    Sint16 * origvx;
    Sint16 * origvy;
    Sint16 * vx;
    Sint16 * vy;
    int size;
    bool error;
    SDL_Surface * texture;

    game_poly(int x, int y, std::vector< std::pair<int, int> > v, SDL_Surface * texture=0);

    void update();
    void draw();

    virtual ~game_poly();
protected:
private:
    void check_move();
    //old x,y : old view x,view y
    int ox,oy,ovx,ovy;
};

#endif // GAME_POLY_H
