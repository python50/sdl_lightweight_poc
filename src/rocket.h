#ifndef rocket_H
#define rocket_H
#include "base.h"
#include "SDL.h"
#include "SDL_gfxPrimitives.h"
#include "Box2D/Box2D.h"

class rocket: public base
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

    rocket(int x, int y, float width=10, float height=10);

    void update();
    void draw();

    virtual ~rocket();
protected:
private:
    void create_body(float w, float h, float a=0);
    void update_v();
    void dampen_xy();
    void check_move();

    //old x,y : old view x,view y
    int ox,oy,ovx,ovy;
    bool started;
};

#endif // rocket_H
