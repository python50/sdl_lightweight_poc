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
#include "Box2D/Box2D.h"
#include "stdio.h"

#include "psrect_static.h"
#include "psrect_dynamic.h"
#include "rocket.h"
#include "time.h"

bool initalize()
{
    srand(time(0));
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    #ifdef WINDOWS_BUILD && DEBUG_BUILD
    //SDL redirects output to stdio.txt and stderr.txt without this
    freopen( "CON", "w", stdout );
	freopen( "CON", "w", stderr );
	#endif

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

    //to draw -> meta::world.DrawDebugData()
    meta::world.SetDebugDraw(new b2Draw());

    return 1;
}

void update_objects()
{

    for(unsigned int i=0; i < meta::objects.size(); i++)
    {
        //std::cout << i << " OBJ-> "<< meta::objects.at(i) <<"\n";
        meta::objects.at(i)->update();
        //std::cout << i << " OBJ-> "<< meta::objects.at(i) <<"\n";
        if (!meta::objects.at(i))
        {
            meta::objects.erase(meta::objects.begin()+i);
            i--;
            std::cout << "Removed\n";
        }
        meta::objects.at(i)->draw();
    }
}

void draw_tile(SDL_Surface * image, int x, int y, float scale=1)
{
    int w=image->w;
    int h=image->h;

    int viewx=scale*meta::view_x;
    int viewy=scale*meta::view_y;

    SDL_Rect rect;
    rect.x=x-(viewx%w);
    rect.y=y-(viewy%h);
    rect.w=image->w;
    rect.h=image->h;

    SDL_BlitSurface(image, 0, meta::screen, &rect);
}

void draw_100()
{
    SDL_Surface * image=get_surface("spaceb");
    int w=image->w;
    int h=image->h;
    for(int i=0; i<(480/h)+3; i++)
    {
        for(int j=0; j<(640/w)+3; j++)
        {
            draw_tile(image, j*w-w, i*h-h, .4);
        }
    }
}

void draw_80()
{
    SDL_Surface * image=get_surface("spacem");
    int w=image->w;
    int h=image->h;
    for(int i=0; i<(480/h)+3; i++)
    {
        for(int j=0; j<(640/w)+3; j++)
        {
            draw_tile(image, j*w-w-4, i*h-h+34, .8);
        }
    }
}

void draw_60()
{
    SDL_Surface * image=get_surface("spacef");
    int w=image->w;
    int h=image->h;
    for(int i=0; i<(480/h)+3; i++)
    {
        for(int j=0; j<(640/w)+3; j++)
        {
            draw_tile(image, j*w-w+15, i*h-h+21, 1.6);
        }
    }
}

void draw_background()
{
    draw_100();
    draw_80();
    draw_60();
}

int main ( int argc, char** argv )
{
	std::cout << "Starting\n";
    initalize();

    //TODO add resource load error handling
    add_surface("spaceb", load_surface("gray_space.png",1)   );
    add_surface("spacem", load_surface("gray_space.png",1)   );
    add_surface("spacef", load_surface("gray_space.png",1)   );

    for(int i=-2; i < 20+2; i++)
        meta::objects.push_back(new psrect_static(200*i, 200, 200, 200));
//meta::objects.push_back(new psrect_static(320, 20, 320, 20));

//meta::objects.push_back(new psrect_static(0, -400,1, 1000));
//meta::objects.push_back(new psrect_static(1000, -400, 1, 1000));

    meta::objects.push_back(new rocket(2000,-100, 50,70));

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
        //SDL_FillRect(meta::screen, 0, SDL_MapRGB(meta::screen->format, 0, 0, 0 ));
        SDL_FillRect(meta::screen, 0, SDL_MapRGB(meta::screen->format, meta::background_red , meta::background_green, meta::background_blue ));

        draw_background();

        // Instruct the world to perform a single step of simulation.
        // It is generally best to keep the time step and iterations fixed.
        meta::world.Step(timeStep, velocityIterations, positionIterations);

        update_objects();
        meta::world.DrawDebugData();

        // DRAWING ENDS HERE

        // finally, update the screen :)
        SDL_Flip(meta::screen);
    } // end main loop

    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}
