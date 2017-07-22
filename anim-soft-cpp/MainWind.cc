#include "headers.h"
#include <iostream>

MainWind::MainWind()
: colSwitchBut(COL_SWITCH_STRING),
  eraseBut(ERASE_STRING),
  selectBut(SELECT_STRING),
  imageBut(IMAGE_STRING),
  exportBut("Export image"),
  saveBut("okay")
{
    set_border_width(GLOBAL_BORDER_WIDTH);

    colSwitchBut.signal_clicked().connect(sigc::mem_fun(*this,
                &MainWind::on_colSwitch_clicked));
    eraseBut.signal_clicked().connect(sigc::mem_fun(*this,
                &MainWind::on_erase_clicked));
    selectBut.signal_clicked().connect(sigc::mem_fun(*this,
                &MainWind::on_select_clicked));
    imageBut.signal_clicked().connect(sigc::mem_fun(*this,
                &MainWind::on_image_clicked));
    exportBut.signal_clicked().connect(sigc::mem_fun(*this,
                &MainWind::on_export_clicked));
    saveBut.signal_clicked().connect(sigc::mem_fun(*this,
                &MainWind::on_Savefile_clicked));

    diag = Gtk::Dialog("Save Anim");
    diag.set_transient_for(*this);
    //Gtk::Image img("apple.png");

    // add to window
    Gtk::Box *bp = diag.get_content_area();
    bp->pack_start(ent);
    bp->pack_start(saveBut);
    bp->show_all();

    grid.attach(colSwitchBut, 0, 0, 1, 1);
    grid.attach(eraseBut, 1, 0, 1, 1);
    grid.attach(selectBut, 2, 0, 1, 1);
    grid.attach(imageBut, 3, 0, 1, 1);
    //grid.attach(img, 0, 1, 1, 1);
    grid.attach(exportBut, 0, 1, 1, 1);

    grid.show_all();
    add(grid);
}

MainWind::~MainWind()
{
}

// toggle
void MainWind::on_colSwitch_clicked()
{
    switchCol();
    //mainLoop();
    /* mark */
   // mainLoop();
}   

void MainWind::on_erase_clicked()
{
    commandSet("eraser");
}

void MainWind::on_select_clicked()
{
    commandSet("select");
}

void MainWind::on_image_clicked()
{
    imageFromFile();
}

void MainWind::on_Savefile_clicked()
{   
    std::cout << "filesave\n";
    saveFile(ent.get_text());
}

void MainWind::on_export_clicked()
{

    ent.set_activates_default(true);
    ent.set_max_length(32);
    ent.set_text("untitled.avi");


    diag.run();
}
    
