#ifndef PROJECT_HEADERS
#define PROJECT_HEADERS

#define NEW_CANVAS_STRING "New Canvas"
#define COL_SWITCH_STRING "Switch Color"


#define GLOBAL_BORDER_WIDTH 10

#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <SFML/Graphics.hpp>



class MainWind : public Gtk::Window
{

public:
    MainWind();
    virtual ~MainWind();

private:
    void  on_colSwitch_clicked();
    Gtk::Button colSwitchBut;
};

class Layer
{
public:
    Layer();
    Layer(sf::Texture text);
    Layer(const Layer& l);

    sf::Texture ltext;

};

class Eraser
{
public:
    Eraser(int size, int startX, int startY);
    int getSize();
    int getX();
    int getY();

    void setPos(int x, int y);


private:
    int size; // size of square
    int x, y;
};



int mainLoop();
void switchCol();

#endif // PROJECT_HEADERS

