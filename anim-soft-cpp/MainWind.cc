#include "headers.h"


MainWind::MainWind()
: colSwitchBut(COL_SWITCH_STRING)
{
    set_border_width(GLOBAL_BORDER_WIDTH);

    colSwitchBut.signal_clicked().connect(sigc::mem_fun(*this,
                &MainWind::on_colSwitch_clicked));

    // add to window
    add(colSwitchBut);
    colSwitchBut.show();
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
