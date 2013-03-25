#include "psrect_static.h"
#include <vector>
#include "meta.h"

psrect_static::psrect_static(int xx, int yy, float ww, float hh)
{
    //ctor
    x=xx;
    y=yy;
    w=ww;
    h=hh;

    std::vector<std::pair<int, int> > v;
    v.push_back(std::pair<int, int> (-w/2,h/2));
    v.push_back(std::pair<int, int> (w/2,h/2));
    v.push_back(std::pair<int, int> (w/2,-h/2));
    v.push_back(std::pair<int, int> (-w/2,-h/2));

    size=v.size();
    origvx=new Sint16[size];
    origvy=new Sint16[size];
    vx=new Sint16[size];
    vy=new Sint16[size];

    for(int i=0; i < size; i++)
    {
        origvx[i]=v.at(i).first;
        origvy[i]=v.at(i).second;
    }

    for(int i=0; i < size; i++)
    {
        vx[i]=v.at(i).first;
        vy[i]=v.at(i).second;
    }

    create_body(w,h);
}

void psrect_static::create_body(float w, float h, float a)
{
    // Define the ground body.
	b2BodyDef BodyDef;
	BodyDef.position.Set(x/10, -y/10);

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	body = meta::world.CreateBody(&BodyDef);

	// Define the ground box shape.
	b2PolygonShape Box;

	// The extents are the half-widths of the box.
	Box.SetAsBox(w/10, h/10, b2Vec2(0,0), a);

	// Add the ground fixture to the ground body.
	body->CreateFixture(&Box, 0.0f);
}

void psrect_static::check_move()
{
    if (ox==x and oy==y and ovx==meta::view_x and ovy==meta::view_y )
        return;

    for(int i=0; i < size; i++)
    {
        vx[i]=origvx[i]+x-meta::view_x;
        vy[i]=origvy[i]+y-meta::view_y;
    }

    ox=x;
    oy=y;
    ovx=meta::view_x;
    ovy=meta::view_y;
}

void psrect_static::update_v()
{
    b2PolygonShape* s=0;
    for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext())
    {
         s= (b2PolygonShape*)f->GetShape();
    }

    if (!s)
    {
        std::cout << "psrect_static::update_v(): error, Body->shape gone ?\n";
        return;
    }

    x=body->GetPosition().x*10;
    y=body->GetPosition().y*-10;


    for(int i=0; i < size and i < s->GetVertexCount(); i++)
    {

        vx[i]=s->GetVertex(i).x*10+x-meta::view_x;
        vy[i]=s->GetVertex(i).y*10+y-meta::view_y;
    }
}

void psrect_static::update()
{
    update_v();
    //check_move();
}

void psrect_static::draw()
{
    filledPolygonRGBA(meta::screen, vx, vy, size, 0x00,0x88,0x88,0xFF);

    pixelColor(meta::screen, x, y, 0x000000FF);
    //stringRGBA(meta::screen, x, y, "sbox", 0X00, 0X00, 0X00, 0X88);
}


psrect_static::~psrect_static()
{
    meta::world.DestroyBody(body);
    //dtor
    delete [] vx;
    delete [] vy;
}
