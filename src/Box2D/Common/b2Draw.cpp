/*
* Copyright (c) 2011 Erin Catto http://box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include <Box2D/Common/b2Draw.h>
#include "meta.h"
#include "SDL_gfxPrimitives.h"

float xmod(float x)
{
    return x*10-meta::view_x;
}

float ymod(float y)
{
    return y*-10-meta::view_y;
}

b2Draw::b2Draw()
{
    m_drawFlags = 0xFFFFFFFF;
}

void b2Draw::SetFlags(uint32 flags)
{
    m_drawFlags = flags;
}

uint32 b2Draw::GetFlags() const
{
    return m_drawFlags;
}

void b2Draw::AppendFlags(uint32 flags)
{
    m_drawFlags |= flags;
}

void b2Draw::ClearFlags(uint32 flags)
{
    m_drawFlags &= ~flags;
}

#define ALPHA_LVL 0x44

/// Draw a closed polygon provided in CCW order.
void b2Draw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    Sint16 * vx=new Sint16[vertexCount];
    Sint16 * vy=new Sint16[vertexCount];

    for(int i=0;i < vertexCount;i++)
    {
        vx[i]=xmod(vertices[i].x);
        vy[i]=ymod(vertices[i].y);
    }

    polygonRGBA(meta::screen, vx, vy, vertexCount, color.r, color.g,  color.b, ALPHA_LVL);
}

/// Draw a solid closed polygon provided in CCW order.
void b2Draw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    Sint16 * vx=new Sint16[vertexCount];
    Sint16 * vy=new Sint16[vertexCount];

    for(int i=0;i < vertexCount;i++)
    {
        vx[i]=xmod(vertices[i].x);
        vy[i]=ymod(vertices[i].y);
    }

    filledPolygonRGBA(meta::screen, vx, vy, vertexCount, color.r, color.g,  color.b, ALPHA_LVL);
}

/// Draw a circle.
void b2Draw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
    circleRGBA(meta::screen, xmod(center.x), ymod(center.y), xmod(radius), color.r, color.g,  color.b, ALPHA_LVL);
    return;
}

/// Draw a solid circle.
void b2Draw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
    filledCircleRGBA(meta::screen, xmod(center.x), ymod(center.y), radius, color.r, color.g,  color.b, ALPHA_LVL);
    return;
}

/// Draw a line segment.
void b2Draw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    lineRGBA(meta::screen, xmod(p1.x), ymod(p1.y), xmod(p2.x), ymod(p2.y), color.r, color.g,  color.b, ALPHA_LVL);
}

/// Draw a transform. Choose your own length scale.
/// @param xf a transform.
void b2Draw::DrawTransform(const b2Transform& xf)
{
    int x1, y1, x2, y2;
    x1=xmod(xf.p.x);
    y1=ymod(xf.p.y);
    x2=x1+xmod(xf.q.c*10);
    y2=y2+ymod(xf.q.s*10);

    //lineRGBA(meta::screen, x1, y1, x2, y2, 0x0, 0x0,  0x0, ALPHA_LVL);
    return;
}
