/*

Algorithm:
Store every vertex created while simultaneously
dragging and pressing the mouse.

On every iteration of the mainloop, draw
a primitive lineStrip connecting all the points(vertices)
stored above.

*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Frame.h"
#include <unistd.h>
#include "globals.h"

//const int WIDTH = 640;
//const int HEIGHT = 480;

const int RAD = 8;
const float FRATE = 100000;

sf::Color col_green(0, 255, 0);
sf::Color col_red(255, 0, 0);
sf::Color fill_col(0, 0, 255);
sf::Color tar_col(255, 255, 255);
sf::Color col_def = col_green;

void switchCol()
{
    if (col_def == col_green)
        col_def = col_red;
    else
        col_def = col_green;
}

// ignoring out-of-bounds errors for now
void floodFill(sf::Image& wind_img, sf::Vector2i nodePos, sf::Color tar, sf::Color rep)
{
    //sf::Image wind_img = text.copyToImage();

    // get color at pos
    sf::Color nodeCol = wind_img.getPixel(nodePos.x, nodePos.y);

    // return if not target
    if (nodeCol != tar)
        return;

    // return if color is pre-present
    if (nodeCol == rep)
        return;

    // the conditions are met for a color change
    wind_img.setPixel(nodePos.x, nodePos.y, rep);
    //text.update(wind_img);

    sf::Vector2i northPos;
    northPos.x = nodePos.x; northPos.y = nodePos.y - 1;
    floodFill(wind_img, northPos, tar, rep);

    sf::Vector2i southPos;
    southPos.x = nodePos.x; southPos.y = nodePos.y + 1;
    floodFill(wind_img, southPos, tar, rep);

    sf::Vector2i eastPos;
    eastPos.x = nodePos.x + 1; eastPos.y = nodePos.y;
    floodFill(wind_img, eastPos, tar, rep);

    sf::Vector2i westPos;
    westPos.x = nodePos.x - 1; westPos.y = nodePos.y;
    floodFill(wind_img, westPos, tar, rep);


}

void saveWindowToFile(sf::Texture text)
{
    sf::Image wind_img = text.copyToImage();
    std::string name;

    std::cout << "Filename: ";
    std::cin >> name;

    wind_img.saveToFile(name);
}

void updateFromWindow(sf::Image& layer_img, sf::Texture wind_text)
{
    sf::Image wind_img = wind_text.copyToImage();

    int i, j;

    // modify all nonwhite pixels
    for (i = 0; i < WIDTH; i++) {
        for (j = 0; j < HEIGHT; j++) {
            
            sf::Color col = wind_img.getPixel(i, j);

            if (col != tar_col) // if not bg col
                layer_img.setPixel(i, j, col);
        }
    }
}



int mainLoop()
{
    // create window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "yattemita");

    //sf::CircleShape shape(50);

    // vector of frames
    std::vector<Frame> frame_list;

    sf::Texture text_pahilo;
    text_pahilo.create(WIDTH, HEIGHT);

    Frame pahilo(text_pahilo);
    frame_list.push_back(pahilo);
    Frame cur_frame = pahilo;

    // vector of circles
    std::vector<sf::Vertex> myVertices;

    std::vector <sf::Sprite>  mySprites[MAX_LAYERS];
    
    sf::Sprite sprite;

    // the texture will store the pixels to be drawn
    // the sprite will be associated with this texutre


    int mousedown = 0;
    int frame_ind = 0;
    int play_mode = 0;
    int layer_num = 0; // first layer
    int li = 0;

    //shape.setFillColor(green);

    window.setFramerateLimit(60);
    while (window.isOpen()) {

        
        li = cur_frame.li; // keep layerind updated
        

        sf::Event event;

        // poll for closing

        // note: haven't specified which button yet
        while (window.pollEvent(event)) {
            
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::A) {
                    saveWindowToFile(cur_frame.layer_list[li].ltext);
                } else if (event.key.code == sf::Keyboard::F) {
                    
                    sf::Vector2i pos = sf::Mouse::getPosition(window);
                    sf::Image img = cur_frame.layer_list[li].ltext.copyToImage();
                    
                    floodFill(img, pos, tar_col, fill_col);
                    cur_frame.layer_list[li].ltext.update(img);
                    /*
                    sf::Image img = texture.copyToImage();
                    img.setPixel(pos.x, pos.y, fill_col);
                    texture.update(img);
                    */

                /* add frame */
                /* only if last frame */
                } else if (event.key.code == sf::Keyboard::Z) {
                    
                    if (frame_ind != frame_list.size() - 1)
                        return -1;

                    // create new frame from cur_frame
                    Frame new_frame(cur_frame);
                    //new_frame = cur_frame; /*change bindings */
                    frame_list.push_back(new_frame);

                    cur_frame = new_frame;
                    frame_ind++;

                } else if (event.key.code == sf::Keyboard::Q) {
                    
                    std::cout << "prev frame \n";

                    if (frame_ind < 1)
                        return -1;

                    frame_ind--;
                    cur_frame = frame_list[frame_ind];
                
                } else if (event.key.code == sf::Keyboard::E) {
                    
                    if (frame_ind >= frame_list.size() - 1)
                        return -1;

                    frame_ind++;
                    cur_frame = frame_list[frame_ind];
                } else if (event.key.code == sf::Keyboard::G) {
                    
                    if (frame_ind == frame_list.size() - 1)
                        std::cout << "Current frame " << std::endl;
                    else 
                        std::cout << "Back/front frame" << std::endl;
                
                } else if (event.key.code == sf::Keyboard::P) {
                    
                    play_mode = 1;
                    frame_ind = 0;
                    cur_frame = frame_list[0];
                
                } 
                    
            }

            else if ((event.type == sf::Event::MouseMoved) && (mousedown == 1)) {
                myVertices.push_back(sf::Vertex(sf::Vector2f(sf::Mouse::getPosition(window)), col_def));
            }

            else if (event.type == sf::Event::MouseButtonPressed) {
                mousedown = 1;
            }

            else if (event.type == sf::Event::MouseButtonReleased) {
                mousedown = 0;
                cur_frame.layer_list[li].ltext.update(window); // copy render target
                myVertices.clear();
            }

        }

        // for playing
        // temporary
        if (play_mode) {
            if (frame_ind < frame_list.size() - 1) {
                

                // just sleep for a while
                usleep(FRATE);
                frame_ind++;
                cur_frame = frame_list[frame_ind];
            } else {
                play_mode = 0;
            }
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // click draw

            

        window.clear(sf::Color::White);

        window.draw(sprite);

        // draw curves on window temp
        window.draw(&myVertices[0], myVertices.size(), sf::LineStrip);

        /*
        sf::Texture temp_tex; 
        temp_tex.create(WIDTH, HEIGHT);
        temp_tex.update(window);

        myVertices.clear();

        sf::Image cur_img = cur_frame.layer_list[li].ltext.copyToImage();
        updateFromWindow(cur_img, temp_tex);

        cur_frame.layer_list[li].ltext.update(cur_img);

        */




        // draw pixels that have yet to be copied to the texture
        // as a LineStrip i.e. connected lines
        //window.draw(&myVertices[0], myVertices.size(), sf::LineStrip);

        /*
        sprite.setTexture(cur_frame.layer_list[li].lext);
        window.draw(sprite);

        sprite.setTexture(cur_frame.layer_list[li].lext2);
        window.draw(sprite);
*/
        sprite.setTexture(cur_frame.layer_list[li].ltext);

        window.display();
    }

    return 7;

}
