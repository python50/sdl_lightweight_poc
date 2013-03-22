#include <cstdlib>

#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

#include "SDL_framerate.h"
#include "SDL_gfxPrimitives.h"
#include "meta.h"
#include "game_poly.h"
#include "map_parser.h"
#include "Box2D.h"

#include "psrect_static.h"
#include "psrect_dynamic.h"

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

    add_surface("cb",load_surface("cb.bmp"));
    SDL_Surface * bmp=get_surface("cb");
    add_surface("metal",load_surface("metal_texture.bmp"));
    SDL_Surface * metal=get_surface("metal");

meta::objects.push_back(new psrect_static(320, 460, 320, 20));
meta::objects.push_back(new psrect_static(320, 20, 320, 20));

meta::objects.push_back(new psrect_static(10, 240,10, 240));
meta::objects.push_back(new psrect_static(630, 240, 10, 240));


meta::objects.push_back(new psrect_dynamic(100,100,20,20));
meta::objects.push_back(new psrect_dynamic(150,100,20,20));
meta::objects.push_back(new psrect_dynamic(200,100,20,20));
meta::objects.push_back(new psrect_dynamic(250,100,20,20));
meta::objects.push_back(new psrect_dynamic(300,100,20,20));
meta::objects.push_back(new psrect_dynamic(350,100,20,20));
meta::objects.push_back(new psrect_dynamic(400,100,20,20));


    //load_map("test.json");

    float32 timeStep = 1.0f / 30.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

    FPSmanager manager;
    SDL_initFramerate(&manager);
    SDL_setFramerate(&manager, 30);

    // program main loop
    bool done = false;
    while (!done)
    {
        SDL_framerateDelay(&manager);

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
        //SDL_FillRect(meta::screen, 0, SDL_MapRGB(meta::screen->format, 0x11, 0x33, 0x55 ));
SDL_FillRect(meta::screen, 0, SDL_MapRGB(meta::screen->format, 0xFF/2, 0xFF/2, 0x88/2 ));

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
