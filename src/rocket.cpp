#include "rocket.h"
#include "psrect_dynamic.h"
#include <vector>
#include "meta.h"
#include "SDL_gfxPrimitives.h"

rocket::rocket(int xx, int yy, float ww, float hh)
{
    //ctor
    x=xx;
    y=yy;
    w=ww;
    h=hh;

    started=0;

    std::vector<std::pair<int, int> > v;
    v.push_back(std::pair<int, int> (-w/2,-h/2));
    v.push_back(std::pair<int, int> (w/2,-h/2));
    v.push_back(std::pair<int, int> (30,h/2));

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

    create_body(w,h,0);
    body->SetAngularDamping(.1f);
}

void rocket::create_body(float w, float h, float a)
{
    b2BodyDef BodyDef;
    BodyDef.type = b2_dynamicBody;
    BodyDef.position.Set(x/10, -y/10);
    body = meta::world.CreateBody(&BodyDef);

    // Define the ground box shape.
    b2PolygonShape Box;

    // The extents are the half-widths of the box.
    //Box.SetAsBox(w/10, h/10, b2Vec2(0,0), a);

    b2Vec2 vertices[3];

    vertices[0].Set(-w/10,-h/10);
    vertices[1].Set(w/10,-h/10);
    vertices[2].Set(0,h/10);

    Box.Set(vertices,3);

    // Define the dynamic body fixture.
    b2FixtureDef FixtureDef;
    FixtureDef.shape = &Box;

    // Set the box density to be non-zero, so it will be dynamic.
    FixtureDef.density = 1;

    // Override the default friction.
    FixtureDef.friction = 0.25f;

    FixtureDef.restitution = 0.3f;

    // Add the shape to the body.
    body->CreateFixture(&FixtureDef);
}

void rocket::check_move()
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

void rocket::update_v()
{
    b2PolygonShape* s=0;
    for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext())
    {
        s= (b2PolygonShape*)f->GetShape();
    }

    if (!s)
    {
        std::cout << "rocket::update_v(): error, Body->shape gone ?\n";
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

void rocket::dampen_xy()
{
    if (x < 0)
    {
        b2Vec2 pos=body->GetPosition();
        pos.x=2000/10;

        body->SetTransform(pos,body->GetAngle());
    }


    if (x > 4000)
    {
        b2Vec2 pos=body->GetPosition();
        pos.x=2000/10;

        body->SetTransform(pos,body->GetAngle());
    }

}

void rocket::update()
{
    update_v();
    //check_move();
    dampen_xy();

    body->SetActive(true);



    static char t=0;
    t+=0x2;

    if (!t and started)
    {
        int j=rand()%100+50;
        for(int i=0; i < j; i++)
        {
            char size= rand()%7+1;
            j-=size;
            meta::objects.push_back(new psrect_dynamic(x-800+rand()%1600, y-1300+rand()%1000, size, size));
        }

    }

    Uint8 *keystate = SDL_GetKeyState(NULL);


    const float vol_up_scale=2.75;
    const float vol_up=400 * vol_up_scale;

    if ( keystate[SDLK_UP] || keystate[SDLK_w] )
    {
        started=1;


        body->SetActive(true);
        float angle=body->GetAngle()+M_PI/2;
        b2Vec2 force = b2Vec2((cos(angle) * vol_up) , (sin(angle) * vol_up));
        body->ApplyForceToCenter(force);

        b2Vec2 velocity = body->GetLinearVelocity();
        float32 speed = velocity.Normalize(); // Normalize returns the length, and set's the length to 1.

        if(speed>7.4)
            body->SetLinearVelocity(.99 * speed * velocity);
    }

    const float vol_side_scale=1;
    const float vol_side=100 * vol_side_scale;


    const float torque=100;
    const float torque_side=torque * .125;

    if ( keystate[SDLK_LEFT] || keystate[SDLK_a] )

    {
        body->ApplyTorque(torque);
    }

    if ( keystate[SDLK_RIGHT] || keystate[SDLK_d] )
    {
        body->ApplyTorque(-torque);
    }

    if ( keystate[SDLK_COMMA] || keystate[SDLK_q] )
    {
        body->ApplyTorque(torque_side);

        float angle=body->GetAngle()+M_PI;
        b2Vec2 force = b2Vec2((cos(angle) * vol_side) , (sin(angle) * vol_side));
        body->ApplyForceToCenter(force);
    }

    if ( keystate[SDLK_PERIOD] || keystate[SDLK_e] )
    {
        body->ApplyTorque(-torque_side);

        float angle=body->GetAngle();
        b2Vec2 force = b2Vec2((cos(angle) * vol_side) , (sin(angle) * vol_side));
        body->ApplyForceToCenter(force);
    }


    meta::view_x=x-320;
    meta::view_y=y-240;

    int red=0;
    int green=0;
    int blue=0;

    if (-y < 3000)
    {
        body->SetGravityScale(1);
        red=    0x100 + round(y*.1276);
        green=  0x110 + round(y*.1255);
        blue=   0x120 + round(y*.1234);

    }
    else if (-y < 6000)
    {
        body->SetGravityScale(.75);
        red=    -.000056888*y*y-.512*y-1024;
        green=  0;
        blue=   0;

    }
    else
        body->SetGravityScale(.5);

    set_background(red, green, blue);
}

void rocket::draw()
{
    filledPolygonRGBA(meta::screen, vx, vy, size, 0xFF,0xDD,0x88,0xFF);


    float ang=body->GetAngle();

    //lineColor(meta::screen, x+cos(ang-M_PI/2)*400-meta::view_x, y-sin(ang-M_PI/2)*400-meta::view_y, x+cos(ang+M_PI/2)*400-meta::view_x, y-sin(ang+M_PI/2)*400-meta::view_y, 0x88FF88FF);


    //Upper Right
    stringRGBA(meta::screen, 10, 10, "ROCKET LAUNCH SIMULATION", 0X88, 0XFF, 0X88, 0XFF);

    if (!started)
    {
        //SAFELY PILOT YOUR SPACECRAFT IN ORDER TO
        //DELIVER YOUR PEOPLE TO THEIR NEW WORLD
        stringRGBA(meta::screen, 10, 30, "  SAFELY PILOT YOUR SPACECRAFT IN ORDER TO", 0X88, 0XFF, 0X88, 0XFF);
        stringRGBA(meta::screen, 10, 40, "  DELIVER YOUR PEOPLE TO THEIR NEW WORLD", 0X88, 0XFF, 0X88, 0XFF);
        stringRGBA(meta::screen, 10, 50, "  ==CONTROLS==", 0X88, 0XFF, 0X88, 0XFF);
        stringRGBA(meta::screen, 10, 60, "  UP - THRUSTERS ON", 0X88, 0XFF, 0X88, 0XFF);
        stringRGBA(meta::screen, 10, 70, "  LEFT - ROTATE LEFT", 0X88, 0XFF, 0X88, 0XFF);
        stringRGBA(meta::screen, 10, 80, "  RIGHT - ROTATE RIGHT", 0X88, 0XFF, 0X88, 0XFF);
        stringRGBA(meta::screen, 10, 90, "  ==GOAL==", 0X88, 0XFF, 0X88, 0XFF);
        stringRGBA(meta::screen, 10, 100, "  REACH THE END WITH OUT DYING, AND THUS DOOMING YOUR ENTIRE RACE", 0X88, 0XFF, 0X88, 0XFF);
    }
    else
    {
            filledRectAlpha(meta::screen, 30,30,30-(y/100),45, 0x88FF8888);
        rectangleColor(meta::screen, 30,30,200,45, 0x88FF88FF);
        stringRGBA(meta::screen, 35, 35, "PROGRESS", 0XFF, 0XFF, 0XFF, 0XFF);

    }

    //Left Lower --------------------------------------------------------
    char buffer[20];

    unsigned int angle=(unsigned int) ((body->GetAngle()+M_PI/2)*180/M_PI)%360;

    int vol=body->GetLinearVelocity().Length()*10;



    stringRGBA(meta::screen, 10, 440, "ANGLE: ", 0X88, 0XFF, 0X88, 0XFF);
    itoa(angle, buffer);
    stringRGBA(meta::screen, 90, 440, buffer, 0X88, 0XFF, 0X88, 0XFF);

    stringRGBA(meta::screen, 10, 450, "VELOCITY: ", 0X88, 0XFF, 0X88, 0XFF);
    itoa(vol, buffer);
    stringRGBA(meta::screen, 90, 450, buffer, 0X88, 0XFF, 0X88, 0XFF);

    stringRGBA(meta::screen, 10, 460, "HEIGHT: ", 0X88, 0XFF, 0X88, 0XFF);
    itoa(-y, buffer);
    stringRGBA(meta::screen, 90, 460, buffer, 0X88, 0XFF, 0X88, 0XFF);

    //Right Upper -----------------------------------------------------------------

    stringRGBA(meta::screen, 435, 10, "COPYRIGHT (C) JASON WHITE", 0X88, 0XFF, 0X88, 0XFF);
    stringRGBA(meta::screen, 435, 20, "       CREATED 03/24/2013", 0X88, 0XFF, 0X88, 0XFF);

    //Left Lower -------------------------------------

    stringRGBA(meta::screen, 550, 440, "X: ", 0X88, 0XFF, 0X88, 0XFF);
    itoa(x, buffer);
    stringRGBA(meta::screen, 570, 440, buffer, 0X88, 0XFF, 0X88, 0XFF);

    stringRGBA(meta::screen, 550, 450, "Y: ", 0X88, 0XFF, 0X88, 0XFF);

    itoa(-y, buffer);
    stringRGBA(meta::screen, 570, 450, buffer, 0X88, 0XFF, 0X88, 0XFF);

    stringRGBA(meta::screen, 550, 460, "Z: ", 0X88, 0XFF, 0X88, 0XFF);
    itoa(0, buffer);
    stringRGBA(meta::screen, 570, 460, buffer, 0X88, 0XFF, 0X88, 0XFF);
}


rocket::~rocket()
{
    meta::world.DestroyBody(body);
    //dtor
    delete [] vx;
    delete [] vy;
}
