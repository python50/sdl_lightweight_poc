#include "psrect_dynamic.h"
#include <vector>
#include "meta.h"

psrect_dynamic::psrect_dynamic(int xx, int yy, float ww, float hh)
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
    //body->ApplyTorque(rand()%50);
    life=600+rand()%300;
}

void psrect_dynamic::create_body(float w, float h, float a)
{
	b2BodyDef BodyDef;
	BodyDef.type = b2_dynamicBody;
	BodyDef.position.Set(x/10, -y/10);
	body = meta::world.CreateBody(&BodyDef);

	// Define the ground box shape.
	b2PolygonShape Box;

	// The extents are the half-widths of the box.
	Box.SetAsBox(w/10, h/10, b2Vec2(0,0), a);

	// Define the dynamic body fixture.
	b2FixtureDef FixtureDef;
	FixtureDef.shape = &Box;

	// Set the box density to be non-zero, so it will be dynamic.
	FixtureDef.density = 1.0f;

	// Override the default friction.
	FixtureDef.friction = 0.1f;

	FixtureDef.restitution = 0.3f;

	// Add the shape to the body.
	body->CreateFixture(&FixtureDef);
}

void psrect_dynamic::check_move()
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

void psrect_dynamic::update_v()
{
    b2PolygonShape* s=0;
    for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext())
    {
         s= (b2PolygonShape*)f->GetShape();
    }

    if (!s)
    {
        std::cout << "psrect_dynamic::update_v(): error, Body->shape gone ?\n";
        return;
    }

    x=body->GetPosition().x*10;
    y=body->GetPosition().y*-10;

    for(int i=0; i < size and i < s->GetVertexCount(); i++)
    {
        vx[i]=body->GetWorldPoint(s->GetVertex(i)).x*10;
        vy[i]=body->GetWorldPoint(s->GetVertex(i)).y*-10;

        vx[i]=vx[i]-meta::view_x;
        vy[i]=vy[i]-meta::view_y;
    }
}

void psrect_dynamic::update()
{
    update_v();
    //check_move();

    life--;
    if (!life)
    {
        remove_object(this);
        return;
    }
}

void psrect_dynamic::draw()
{
    filledPolygonRGBA(meta::screen, vx, vy, size, 0xBB,0x55,0x00,0xFF);

}


psrect_dynamic::~psrect_dynamic()
{
    meta::world.DestroyBody(body);
    //dtor
    delete [] vx;
    delete [] vy;
}
