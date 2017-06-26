#include "headers.h"

Layer::Layer(sf::Texture text)
{
    ltext = text;
}


Layer::Layer(const Layer& l)
{
    sf::Texture tex(l.ltext);
    ltext = tex;
}

