#include "headers.h"

LayerWind::LayerWind()
{
    Gtk::Image img("apple.png");

    grid.attach(img, 0, 0, 1, 1);
    grid.show_all();

    add(grid);
}

LayerWind::~LayerWind()
{
}
