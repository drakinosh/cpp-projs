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


// for SelectBox

SelectBox::SelectBox(int sx, int sy, int w, int h)
{
   x = sx; y = sy;
   width = w;
   height = h;
}

void SelectBox::setPos(int newX, int newY)
{
    x = newX;
    y = newY;
}

void SelectBox::setSize(int newW, int newH)
{
    width = newW;
    height = newH;
}

int SelectBox::getX() { return x; }
int SelectBox::getY() { return y; }
int SelectBox::getW() { return width; }
int SelectBox::getH() { return height; }
