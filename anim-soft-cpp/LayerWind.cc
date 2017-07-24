#include "headers.h"
#include "globals.h"
#include <gdkmm/pixbuf.h>

LayerWind::LayerWind()
{
    lout = Gtk::Layout();
    sw = Gtk::ScrolledWindow();
    //m_adjustment->set_value(0);
    //m_adjustment->set_upper(20);
    //m_adjustment->set_lower(0);
    m_adjustment = Gtk::Adjustment::create(0.0, 0.0, 101.0, 0.1, 1.0, 1.0);
    sw.set_hadjustment(m_adjustment);
    sw.set_policy(Gtk::POLICY_ALWAYS, Gtk::POLICY_NEVER);
    //h_scrollbar(adj_scroll);
    auto p1 = Gdk::Pixbuf::create_from_file("unt_0.jpg");
    auto p2 = Gdk::Pixbuf::create_from_file("unt_1.jpg");

    auto s1 = p1->scale_simple(WIDTH/4, HEIGHT/4, Gdk::INTERP_BILINEAR);
    auto s2 = p2->scale_simple(WIDTH/4, HEIGHT/4, Gdk::INTERP_BILINEAR);

    img.set(s1);
    img2.set(s2);

    set_default_size(WIDTH, HEIGHT/4);
    grid.attach(img, 0, 0, 1, 1);
    grid.attach(img2,1, 0, 1, 1);
    //grid.attach(h_scrollbar, 0, 1, 1, 1);
    grid.show_all();

    sw.add(grid);
    sw.show_all();
    add(sw);
    /*
    lout.add(grid);
    lout.show_all();
    add(lout);
    */

    //add(grid);
}

LayerWind::~LayerWind()
{
}
