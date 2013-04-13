#include "meta.h"
#include "Box2D.h"
#include "SDL_image.h"

namespace meta
{
int screen_width=0;
int screen_height=0;
int view_x=0;
int view_y=0;
int map_width=0;
int map_height=0;
SDL_Surface * screen=0;
b2Vec2 gravity(0.0f, -15.0f);
b2World world(gravity);
std::vector<std::pair<std::string , SDL_Surface *> > surfaces;
std::vector<base *> objects;
char background_red=0;
char background_green=0;
char background_blue=0;
}

SDL_Surface * load_surface(std::string filename, char mode)
{
    const char * prefix="load_surface(): ";

    SDL_Surface * data=IMG_Load(filename.c_str());

    if (data==0)
    {
        std::cout << prefix << "Could Not Load `" << filename << "`, `" << SDL_GetError() <<"`\n";
        return 0;
    }

    //if (mode==1)
    //{
            long colorkey = SDL_MapRGB(meta::screen->format, 0, 0, 0);
            SDL_SetColorKey(data, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
    //}



    return SDL_DisplayFormat(data);
}

void add_surface(std::string id, SDL_Surface * surface)
{
    //Don't add null sufaces
    if (!surface)
        return;

    std::pair<std::string, SDL_Surface *> item(id, surface);
    meta::surfaces.push_back(item);
}

SDL_Surface * get_surface(std::string id)
{
    const char * prefix="get_surface(): ";

    for (unsigned int i=0; i <= meta::surfaces.size()-1 ; i++)
    {

        if ( meta::surfaces.at(i).first==id)
        {
            if ((meta::surfaces.size()-1) >= i)
                return meta::surfaces.at(i).second;
        }

    }

    std::cout << prefix <<  "Surface `" << id << "` not found\n";
    return NULL;
}

bool collide_rect(SDL_Rect a , SDL_Rect b)
{
    if(b.x + b.w < a.x)	return 0;	//just checking if their
    if(b.x > a.x + a.w)	return 0;	//bounding boxes even touch

    if(b.y + b.h < a.y)	return 0;
    if(b.y > a.y + a.h)	return 0;

    return 1;	//bounding boxes intersect
}

//darn, its hard to be lazy about program design, *must resist the urge to make four or more blit functions*
void blit_game(SDL_Surface * image, int x, int y)
{
    if (!image)
        return;

    SDL_Rect rect;
    rect.x=x-meta::view_x;
    rect.y=y-meta::view_y;;
    rect.w=image->w;
    rect.h=image->h;

    SDL_BlitSurface(image, 0, meta::screen, &rect);
}

void blit_screen(SDL_Surface * image, int x, int y)
{
    if (!image)
        return;

    SDL_Rect rect;
    rect.x=x-meta::view_x;
    rect.y=y-meta::view_y;;
    rect.w=image->w;
    rect.h=image->h;

    SDL_BlitSurface(image, 0, meta::screen, &rect);
}

const float b2_scaling_factor=10;
std::vector<std::pair<int, int> > b2poly_convert(b2PolygonShape * shape)
{
	std::vector<std::pair<int, int> > v;
	for(int i=0;i < shape->GetVertexCount();i++)
	{
	    std::pair<int, int> p;
	    p.first=shape->GetVertex(i).x*b2_scaling_factor;
	    p.second=shape->GetVertex(i).y*-b2_scaling_factor;
	    v.push_back(p);

	    std::cout << "->" << p.first << " " << p.second << "\n";
	}
	return v;
}


#define base 10

char * uitoa(unsigned long val, char * buf)
{
	unsigned	long	v;
	char		c;

	v = val;
	do {
		v /= base;
		buf++;
	} while(v != 0);
	*buf-- = 0;
	do {
		c = val % base;
		val /= base;
		if(c >= 10)
			c += 'A'-'0'-10;
		c += '0';
		*buf-- = c;
	} while(val != 0);
	return buf;
}


char * itoa(long val, char * buf)
{
	char *	cp = buf;

	if(val < 0) {
		*buf++ = '-';
		val = -val;
	}

	uitoa(val, buf);
	return cp;
}


bool remove_object(void * ptr)
{
    for(unsigned int i=0; i < meta::objects.size(); i+=1)
    {
        if (meta::objects.size()==0)
            break;

        if (meta::objects.at(i)==NULL)
            continue;

        if ((void *)meta::objects.at(i)==ptr)
        {
            delete meta::objects.at(i);
            meta::objects.at(i)=0;
            //meta::objects.erase(meta::objects.begin()+i);
            return 1;
        }
    }

    return 0;
}

char limit_char(int x)
{
    if (x>=0xFF)
        return 0xFF;
    else if (x<=0)
        return 0;
    else
        return x;
}

void set_background(int red, int green, int blue)
{

    meta::background_red=limit_char(red);
    meta::background_green=limit_char(green);
    meta::background_blue=limit_char(blue);
}
