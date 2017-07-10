#include "headers.h"


MainWind::MainWind()
: colSwitchBut(COL_SWITCH_STRING),
  eraseBut(ERASE_STRING),
  selectBut(SELECT_STRING),
  imageBut(IMAGE_STRING)
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



    // add to window
    grid.attach(colSwitchBut, 0, 0, 1, 1);
    grid.attach(eraseBut, 1, 0, 1, 1);
    grid.attach(selectBut, 2, 0, 1, 1);
    grid.attach(imageBut, 3, 1, 1, 1);

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
