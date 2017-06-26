#include <SFML/Graphics.hpp>
#include "headers.h"

// the class Frame
// A frame only has images currently
// in the future, associated object data must also be stored

class Frame {

private:
    sf::Image onion_img;

public:
    sf::Texture text;   // currently public due to referencing problems


    int li;   // layer index
    int num_layers;
    std::vector <Layer> layer_list;

    // no current layer
    // refere to layer by layer_list[layer_ind]

    Frame(sf::Texture text); // initialiser
    Frame(const Frame& f);

    void addLayer();


};
