#include "game_poly.h"
#include "meta.h"

game_poly::game_poly(int xx, int yy, std::vector< std::pair<int, int> > v, SDL_Surface * ntexture)
{
    //ctor
    error=0;

    x=xx;
    y=yy;

    if (v.size() < 3)
    {
        if (v.size()==2 and v.at(0).first==0 and v.at(0).second==0)
        {
            //Build a box
            int w=v.at(1).first;
            int h=v.at(1).second;
            v.clear();
            v.push_back(std::pair<int, int> (0,0));
            v.push_back(std::pair<int, int> (w,0));
            v.push_back(std::pair<int, int> (w,h));
            v.push_back(std::pair<int, int> (0,h));
            std::cout << "built a box of " << w << " " << h << "\n";
        }
        else
        {
            error=1;
            std::cout << "game_poly: error, initalized with less then 3 vertices, and is not of box type";
        }
    }

    size=v.size();
    vx=new Sint16[size];
    vy=new Sint16[size];
    origvx=new Sint16[size];
    origvy=new Sint16[size];

    for(int i=0; i < size; i++)
    {
        origvx[i]=v.at(i).first;
        origvy[i]=v.at(i).second;
    }

    for(int i=0; i < size; i++)
    {
        vx[i]=origvx[i];
        vy[i]=origvy[i];
    }

    texture=ntexture;
}

void game_poly::check_move()
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

    std::cout << x << " " << y << " " << vx[0] << " " << vy[0] << "\n";
}

void game_poly::update_v()
{/*
    for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext())
    {
        ....
    }*/
}

void game_poly::update()
{
    check_move();
}

void game_poly::draw()
{
    if (error)
        return;


    if (!texture)
        filledPolygonRGBA(meta::screen, vx, vy, size, 0xFF,0xFF,0xFF,0xFF);
    else
        texturedPolygon(meta::screen, vx, vy, size, texture, origvx[0]+x-meta::view_x, origvy[0]+y+meta::view_y);


}

game_poly::~game_poly()
{
    //dtor
    delete [] vx;
    delete [] vy;
}
