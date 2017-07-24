#ifndef PROJECT_HEADERS
#define PROJECT_HEADERS

#define NEW_CANVAS_STRING "New Canvas"
#define COL_SWITCH_STRING "Switch Color"
#define ERASE_STRING "Eraser"
#define SELECT_STRING "Select"
#define IMAGE_STRING "Load image"
#define NIL "NIL"

#define GLOBAL_BORDER_WIDTH 10

#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/grid.h>
#include <gtkmm/image.h>
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gdkmm/pixbuf.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/scrollbar.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/layout.h>
#include <glibmm/refptr.h>
#include <SFML/Graphics.hpp>
#include <gtkmm.h>



class MainWind : public Gtk::Window
{

public:
    MainWind();
    virtual ~MainWind();

private:
    void on_colSwitch_clicked();
    void on_erase_clicked();
    void on_select_clicked();
    void on_image_clicked();
    void on_export_clicked();
    void on_Savefile_clicked();

    Gtk::Button colSwitchBut;
    Gtk::Button eraseBut;
    Gtk::Button selectBut;
    Gtk::Button imageBut;
    Gtk::Button exportBut;
    Gtk::Button saveBut;
    Gtk::Grid grid;
    Gtk::Entry ent;
    Gtk::Dialog diag;
};


class LayerWind : public Gtk::Window
{
public:
    LayerWind();
    virtual ~LayerWind();

private:
    Gtk::Grid grid;
    Gtk::Image img;
    Gtk::Image img2;
    Gtk::Layout lout;
    Gtk::ScrolledWindow sw;
    Glib::RefPtr<Gtk::Adjustment> m_adjustment;
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

class SelectBox
{
public:
    SelectBox(int startX, int startY, int w, int h);
    int getX(); int getY();
    int getW(); int getH();
    void setPos(int x, int y);
    void setSize(int w, int h);
    sf::RectangleShape sebox_rect;

private:
    int width, height;
    int x, y;
};



int mainLoop();
void switchCol();
void commandSet(std::string s);
void imageFromFile();
void saveFile(std::string fname);

#endif // PROJECT_HEADERS

