#ifndef psrect_static_H
#define psrect_static_H
#include "base.h"
#include "SDL.h"
#include "SDL_gfxPrimitives.h"
#include "Box2D/Box2D.h"

class psrect_static : public base
{
    public:
    int x;
    int y;
    int w;
    int h;
    Sint16 * vx;
    Sint16 * vy;
    Sint16 * origvx;
    Sint16 * origvy;
    int size;

    b2Body* body;

    psrect_static(int x, int y, float width, float height);

    void update();
    void draw();

    virtual ~psrect_static();
protected:
private:
    void create_body(float w, float h, float a=0);
    void update_v();
    void check_move();

    //old x,y : old view x,view y
    int ox,oy,ovx,ovy;
};

#endif // psrect_static_H
