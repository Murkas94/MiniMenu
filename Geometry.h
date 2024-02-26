#ifndef MINIMENU_GEOMETRY_H
#define MINIMENU_GEOMETRY_H

namespace MiniMenu
{
    struct Vec2
    {
        int16_t x;
        int16_t y;

        Vec2():x(0),y(0){}
        Vec2(int16_t x, int16_t y) : x(x), y(y) {}
        Vec2& operator=(const Vec2& o){x=o.x; y=o.y; return *this;}
        Vec2& operator+=(const Vec2& o){x+=o.x; y+=o.y; return *this;}
        Vec2& operator-=(const Vec2& o){x-=o.x; y-=o.y; return *this;}
        Vec2 operator+(const Vec2& o)const{return Vec2(x+o.x, y+o.y);}
        Vec2 operator-(const Vec2& o)const{return Vec2(x-o.x, y-o.y);}
    };

    struct Rect
    {
        Vec2 pos;
        Vec2 size;

        Rect() : pos(), size(){}
        Rect(int16_t x, int16_t y, int16_t w, int16_t h)
            :pos(x,y), size(w,h){}
        Rect(const Vec2& pos, const Vec2& size)
            :pos(pos),size(size){}
    };
}

#endif
