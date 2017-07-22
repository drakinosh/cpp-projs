#include "headers.h"
#include <gtkmm/application.h>
#include <thread>

int main(int argc, char *argv[])
{

    auto app = Gtk::Application::create(argc, argv, "org.dude");
    std::thread t1(mainLoop);
   
    //int rc2 = pthread_create(&sfml_thread, NULL, app->run, (void *)mw);

    MainWind mw;
    mw.show();

    //LayerWind lw;
    //lw.show();

    //app->add_window(mw);

    //t1.join();
    return app->run(mw);

}
