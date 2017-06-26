#include "Frame.h"
#include "globals.h"
#include "headers.h"

Frame::Frame(sf::Texture texture)
{
    Layer first_l(texture);
    layer_list.push_back(first_l);
    li = 0;

}

// copy cons
// copy all layers of prev frame
Frame::Frame(const Frame& f)
{

    for (auto layer: f.layer_list) {
        Layer temp(layer); // create temp holding name
        layer_list.push_back(temp); // push all layers into list
    }

    li = f.li;
        
}

void Frame::addLayer()
{
    sf::Texture new_text;
    new_text.create(WIDTH, HEIGHT);
    Layer new_layer(new_text);

    layer_list.push_back(new_layer);

    num_layers += 1;
    li = num_layers - 1;

    
}
