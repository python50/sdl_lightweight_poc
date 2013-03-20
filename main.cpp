#include <cstdlib>

#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

#include "SDL_gfxPrimitives.h"
#include "meta.h"
#include "game_poly.h"
#include "map_parser.h"
#include "Box2D.h"

bool initalize()
{
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    meta::screen_width=640;
    meta::screen_height=480;
    // create a new window
    meta::screen  = SDL_SetVideoMode(640, 480, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if ( !meta::screen )
    {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }
    return 1;
}


void init_box2d()
{
	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(30.0f, -40.0f);

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = meta::world.CreateBody(&groundBodyDef);

	// Define the ground box shape.
	b2PolygonShape groundBox;

	// The extents are the half-widths of the box.
	groundBox.SetAsBox(20.0f, 1.0f, b2Vec2(0,0), .05f);

	// Add the ground fixture to the ground body.
	groundBody->CreateFixture(&groundBox, 0.0f);


	game_poly * poly =new game_poly(0,-100, b2poly_convert(&groundBox));
	meta::objects.push_back(poly);

poly->x=groundBody->GetPosition().x*10+5*10;
poly->y=groundBody->GetPosition().y*-10+1*10;
}

void update_objects()
{
    for(unsigned int i=0; i < meta::objects.size(); i++)
    {
        meta::objects.at(i)->update();
        meta::objects.at(i)->draw();
    }
}

int main ( int argc, char** argv )
{
    initalize();
    init_box2d();

    add_surface("cb",load_surface("cb.bmp"));
    SDL_Surface * bmp=get_surface("cb");
    add_surface("metal",load_surface("metal_texture.bmp"));
    SDL_Surface * metal=get_surface("metal");



	b2BodyDef ballBodyDef;
	ballBodyDef.type = b2_dynamicBody;
	ballBodyDef.position.Set(45.0f, -4.0f);
	b2Body* ballBody = meta::world.CreateBody(&ballBodyDef);

	// Define the ground box shape.
	b2PolygonShape ballBox;

	// The extents are the half-widths of the box.
	ballBox.SetAsBox(1.0f, 1.0f);

	// Define the dynamic body fixture.
	b2FixtureDef ballFixtureDef;
	ballFixtureDef.shape = &ballBox;

	// Set the box density to be non-zero, so it will be dynamic.
	ballFixtureDef.density = 1.0f;

	// Override the default friction.
	ballFixtureDef.friction = 0.0f;

	ballFixtureDef.restitution = 0.1f;

	// Add the shape to the body.
	ballBody->CreateFixture(&ballFixtureDef);

	// Add the ground fixture to the ground body.
	//ballBody->CreateFixture(&ballBox, 0.0f);


	game_poly * poly =new game_poly(0,0,b2poly_convert(&ballBox));













    //load_map("test.json");

    float32 timeStep = 1.0f / 30.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;


    // program main loop
    bool done = false;
    while (!done)
    {
        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // check for messages
            switch (event.type)
            {
                // exit if the window is closed
            case SDL_QUIT:
                done = true;
                break;

                // check for keypresses
            case SDL_KEYDOWN:
                // exit if ESCAPE is pressed
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    done = true;
                break;
            } // end switch
        } // end of message processing

        // DRAWING STARTS HERE

        // clear screen
        SDL_FillRect(meta::screen, 0, SDL_MapRGB(meta::screen->format, 0x55, 0x55, 0x55));


		// Instruct the world to perform a single step of simulation.
		// It is generally best to keep the time step and iterations fixed.
		meta::world.Step(timeStep, velocityIterations, positionIterations);


//delete poly;

//poly =new game_poly(0,0,b2poly_convert(&ballBox));

poly->x=ballBody->GetPosition().x*10+1*10;;
poly->y=ballBody->GetPosition().y*-10+1*10;;

std::cout << "POS _> " << poly->x << " " << poly->y << "\n";

poly->update();
poly->draw();

        //update_objects();

        // DRAWING ENDS HERE

        // finally, update the screen :)
        SDL_Flip(meta::screen);
    } // end main loop

    // free loaded bitmap
    SDL_FreeSurface(bmp);

    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}
