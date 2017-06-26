#include "headers.h"


Eraser::Eraser(int sz, int startx, int starty)
{
    size = sz;
    x = startx; y = starty;

}

int Eraser::getSize()
{
    return size;
}

void Eraser::setPos(int newX, int newY)
{   
    x = newX;
    y = newY;
}

int Eraser::getX() { return x; }
int Eraser::getY() { return y; }
