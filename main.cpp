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
	groundBodyDef.position.Set(0.0f, -10.0f);

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = meta::world.CreateBody(&groundBodyDef);

	// Define the ground box shape.
	b2PolygonShape groundBox;

	// The extents are the half-widths of the box.
	groundBox.SetAsBox(5.0f, 1.0f);

	// Add the ground fixture to the ground body.
	groundBody->CreateFixture(&groundBox, 0.0f);


	game_poly * poly =new game_poly(0,0,b2poly_convert(&groundBox));
	meta::objects.push_back(poly);
}

void update_objects()
{
    for(unsigned int i=0; i < meta::objects.size(); i++)
    {
        std::cout << i << " " << meta::objects.size() << "\n";
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

    //load_map("test.json");

    float32 timeStep = 1.0f / 60.0f;
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

        update_objects();

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
