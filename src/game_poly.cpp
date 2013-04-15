#include "game_poly.h"
#include "meta.h"

game_poly::game_poly(int xx, int yy, std::vector< std::pair<int, int> > vlist, SDL_Surface * ntexture)
{
    //ctor
    error=0;

    x=xx;
    y=yy;

    b2BodyDef BodyDef;
	BodyDef.type = b2_dynamicBody;
	BodyDef.position.Set(x/10, -y/10);

	body = meta::world.CreateBody(&BodyDef);
    b2Vec2 vertices[vlist.size()]; //=malloc( sizeof(b2Vec2 *) *  );
    size=vlist.size();

    for(uint16 i=0;i < size;i++)
        vertices[i]=b2Vec2( vlist.at(i).first
                           ,vlist.at(i).second);

	// Define the ground box shape.
	b2PolygonShape Box;

	Box.Set(vertices, size);


    texture=ntexture;
    body=0;
}

void game_poly::update_v()
{

    b2PolygonShape* s=0;
    for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext())
    {
         s= (b2PolygonShape*)f->GetShape();
    }

    x=body->GetPosition().x*10;
    y=body->GetPosition().y*-10;

    for(int i=0; i < size and i < s->GetVertexCount(); i++)
    {
        vx[i]=body->GetWorldPoint(s->GetVertex(i)).x*10;
        vy[i]=body->GetWorldPoint(s->GetVertex(i)).y*-10;

        if (i==0)
        {
            origx=vx[i];
            origy=vy[i];
        }


        vx[i]=vx[i]-meta::view_x;
        vy[i]=vy[i]-meta::view_y;
    }

}

void game_poly::update()
{
    update_v();
}

void game_poly::draw()
{
    if (error)
        return;


    if (!texture)
        filledPolygonRGBA(meta::screen, vx, vy, size, 0xFF,0xFF,0xFF,0xFF);
    else
        texturedPolygon(meta::screen, vx, vy, size, texture, origx+x-meta::view_x, origy+y+meta::view_y);

    pixelColor(meta::screen, x, y, 0x000000FF);
}

game_poly::~game_poly()
{
    //dtor
    delete [] vx;
    delete [] vy;
}
