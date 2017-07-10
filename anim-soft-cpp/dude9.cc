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

bool eraser_mode = false;
bool select_mode = false;
bool draw_mode = true;
bool toLoad = false;

sf::Image importImage;

const int DEBUG = 1;

sf::Color col_green(0, 255, 0);
sf::Color col_red(255, 0, 0);
sf::Color fill_col(0, 0, 255);
sf::Color tar_col(255, 255, 255);
sf::Color col_def = col_green;

std::string command;

void switchCol()
{
    if (col_def == col_green)
        col_def = col_red;
    else
        col_def = col_green;
}

// eraser or select
void commandSet(std::string s)
{
    command = s;

    // if already in said mode,
    // toggle

    if (eraser_mode && s == "eraser")
        command = "neraser";
    if (select_mode && s == "select")
        command = "nselect";

    
}

void imageFromFile()
{
    std::string fname;
    std::cout << "Filename: ";
    std::cin >> fname;

    importImage.loadFromFile(fname);
    toLoad = true;

    std::cout << "Select topLeft corner of image.\n";

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



void getTotalImage(sf::Image i1, sf::Image i2, sf::Image& i)
{
    /* draw i2 over i1 */
    int x, y;

    sf::Color pix1, pix2;

    for (x = 0; x < WIDTH; x++) {
        
        for (y = 0; y < HEIGHT; y++) {

            pix1 = i1.getPixel(x, y);
            pix2 = i2.getPixel(x, y);

            i.setPixel(x, y, pix1);

            if (pix2 != tar_col) {
                
                if (DEBUG) { 
                    std::cout << "overwritten!!\n";
                }

                i.setPixel(x, y, pix2);
            }

        }

    }

}

void eraseAtCoords(Eraser e, sf::Texture& text)
{
    int sz = e.getSize();
    int x = e.getX(); int y = e.getY();

    sf::Image img = text.copyToImage();

    // set all pixels to white
    // mousePos is at topleft corner

    int i, j;

    if (DEBUG) {
        std::cout << "x for " << x << " to " << x +sz << std::endl;
    }


    for (i  = x; i <= x + sz; i++) {
        for (j = y; j <= y + sz; j++) {
            img.setPixel(i, j, tar_col);
        }
    }

    text.update(img);

}

bool isInsideRect(int x1, int y1, SelectBox sb)
{
    int x = sb.getX(); int y = sb.getY();
    int w = sb.getW(); int h = sb.getH();

    if (x1 > x && x1 < x + w && y1 > y && y1 < y + h)
        return true;

    return false;
}
void moveImage(int x, int y, int x1, int y1, sf::Texture& text, SelectBox sb)
{
    // sanity check: exit if two positions are one and the same
    if (x == x1 && y == y1)
        return;

    sf::Image im = text.copyToImage();
    sf::Image back(im); // copy cons
    
    // change pixels in rect area (new)
    int i, j;
    int w = sb.getW();
    int h = sb.getH();

    // set prev rect to white
    for (i = x; i <= x + w; i++)
        for (j = y; j <= y + h; j++)
            im.setPixel(i, j, tar_col);

    for (i = x1; i <= x1 + w; i++) {
        for (j = y1; j <= y1 + h; j++) {
     
            // set it to corresponding prev pixel
            sf::Color prev = back.getPixel(x + (i-x1), y + (j -y1));

            //im.setPixel(x + (i - x1), y + (j - y1), tar_col);
            im.setPixel(i, j, prev);
            // set prev pixel to white
            //im.setPixel(x + (i-x1), y + (j-y1), tar_col);
        }
    }


    text.update(im);

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

    //std::vector <sf::Sprite>  mySprites[MAX_LAYERS];

    sf::Sprite mySprites[MAX_LAYERS];
    
    sf::Sprite sprite;

    // the texture will store the pixels to be drawn
    // the sprite will be associated with this texutre


    int mousedown = 0;
    int frame_ind = 0;
    int play_mode = 0;
    int layer_num = 0; // first layer
    int li = 0;
    
    /*
    bool eraser_mode = false;
    bool draw_mode = true;
    bool select_mode = false;
    */

    
    Eraser eraser(10, 0, 0);
    SelectBox seBox(0, 0, 0, 0);
    
    // first click, drag, last click
    // to create the select box

    int sebox_fclick = 0;
    int sebox_lclick = 0;

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
                    int old = cur_frame.li;
                    cur_frame = frame_list[frame_ind];
                    cur_frame.li = old;
                
                } else if (event.key.code == sf::Keyboard::E) {
                    
                    if (frame_ind >= frame_list.size() - 1)
                        return -1;

                    int old = cur_frame.li;
                    frame_ind++;
                    cur_frame = frame_list[frame_ind];
                    cur_frame.li = old;

                } else if (event.key.code == sf::Keyboard::G) {
                    
                    std::cout << "Frame: " << frame_ind << std::endl;
                    std::cout << "layer: " << li << std::endl;
                
                } else if (event.key.code == sf::Keyboard::P) {
                    
                    play_mode = 1;
                    frame_ind = 0;
                    cur_frame = frame_list[0];
                
                } else if (event.key.code == sf::Keyboard::M) {
                    
                    if (cur_frame.num_layers <=  MAX_LAYERS) {
                        cur_frame.addLayer();
                        li = cur_frame.li;
                        sprite.setTexture(cur_frame.layer_list[li].ltext);
                    }
                
                } else if (event.key.code == sf::Keyboard::S) {
                    std::cout << "Enter layer index\n";
                    int layer_ind;

                    std::cin >> layer_ind;

                    cur_frame.li = layer_ind;
                    li = cur_frame.li;
                    sprite.setTexture(cur_frame.layer_list[li].ltext);
                
                } else if (event.key.code == sf::Keyboard::C) {
                    
                    std::cout << "command: ";
                    std::cin >> command;

                    /*

                    if (command == "eraser") {
                        eraser_mode = true;
                        draw_mode = false;

                        std::cout << "eraser_mode!\n";

                    } else if (command == "neraser") {
                        eraser_mode = false;
                        draw_mode = true;
                    
                    } else if (command == "select") {
                        select_mode = true;
                        draw_mode = false;
                        std::cout << "select_mode!\n";

                    } else if (command == "nselect") {
                        select_mode = false;
                        draw_mode = true;

                        // back to square one;
                        sebox_fclick = 0;
                        sebox_lclick = 0;
                        seBox.setPos(0, 0);
                        seBox.setSize(0, 0);

                    }
                    */

                
                }




            }


            else if ((event.type == sf::Event::MouseMoved) && (mousedown == 1)) {
                
                if (draw_mode) {
                    myVertices.push_back(sf::Vertex(sf::Vector2f(sf::Mouse::getPosition(window)), col_def));
                
                } 
                else if (eraser_mode) {
                    //eraseAtCoords(sf::Mouse::getPosition(window), cur_frame.layer_list[li].ltext);
                    eraseAtCoords(eraser, cur_frame.layer_list[li].ltext);
                }
                
                else if (select_mode && sebox_fclick && !sebox_lclick) {
                    // draw select box as it is being dragged
                    sf::Vector2i pos = sf::Mouse::getPosition(window);
                    seBox.setSize(pos.x - seBox.getX(), pos.y - seBox.getY());
                }

                // drag select box around
                // note: currently buggy, image inside box disappears on dragging
                // will fix this soon
                /*
                else if (select_mode && sebox_lclick) {
                    
                    sf::Vector2i pos = sf::Mouse::getPosition(window);

                    if ((pos.x - seBox.getX()) > 0 || (pos.y-seBox.getY()) > 0) {
                        moveImage(seBox.getX(), seBox.getY(), pos.x, pos.y,
                                    cur_frame.layer_list[li].ltext, seBox);
                        seBox.setPos(pos.x, pos.y);
                    }

                }
                */
                
                
            }

            else if (event.type == sf::Event::MouseButtonPressed) {
                mousedown = 1;

                // erase
                if (eraser_mode) {
                    eraseAtCoords(eraser, cur_frame.layer_list[li].ltext);
                }
                // start of select
                else if (select_mode) {
                    if (!sebox_fclick) {
                        sf::Vector2i ps = sf::Mouse::getPosition(window);
                        seBox.setPos(ps.x, ps.y);
                        sebox_fclick = 1;

                        if (DEBUG) {
                            std::cout << "sebox flick accom.\n";
                        }
                    }

                    if (sebox_lclick) {
                        sf::Vector2i pos = sf::Mouse::getPosition(window);

                        // move the selected are iff the new position is 
                        // not inside the select box

                        if (!isInsideRect(pos.x, pos.y, seBox)) {
                            moveImage(seBox.getX(), seBox.getY(), pos.x, pos.y,
                                        cur_frame.layer_list[li].ltext, seBox);

                            if (DEBUG)
                                std::cout << "X change: " << pos.x - seBox.getX();

                            seBox.setPos(pos.x, pos.y);
                        }
                    }
                        
                        
                } else if (toLoad) {
                    sf::Vector2i pos = sf::Mouse::getPosition(window);
                    toLoad = false;
                    cur_frame.layer_list[li].ltext.update(importImage, pos.x, pos.y);

                    if (DEBUG)
                        std::cout << "Imported!\n";
                }

            }

            else if (event.type == sf::Event::MouseButtonReleased) {
                mousedown = 0;
                if (!play_mode && !eraser_mode && !select_mode && draw_mode)
                    cur_frame.layer_list[li].ltext.update(window); // copy render target

                // set selectbox size
                // if it has been dragged and  released
                if (select_mode && sebox_fclick && !sebox_lclick) {
                    sebox_lclick = 1;
                    sf::Vector2i p = sf::Mouse::getPosition(window);
                    int w = p.x - seBox.getX();
                    int h = p.y - seBox.getY();
                    seBox.setSize(w, h);

                    if (DEBUG) {
                        std::cout << "sebox lclick accom.\n";
                    }
                }
                myVertices.clear();
            }

        }

        // check commands
        if (command == "eraser") {
                eraser_mode = true;
                draw_mode = false;

                std::cout << "eraser_mode!\n";

            } else if (command == "neraser") {
                eraser_mode = false;
                draw_mode = true;
            
            } else if (command == "select") {
                select_mode = true;
                draw_mode = false;
                std::cout << "select_mode!\n";

            } else if (command == "nselect") {
                select_mode = false;
                draw_mode = true;

                // back to square one;
                sebox_fclick = 0;
                sebox_lclick = 0;
                seBox.setPos(0, 0);
                seBox.setSize(0, 0);

            }else if (command == "eraser") {
                eraser_mode = true;
                draw_mode = false;

                std::cout << "eraser_mode!\n";

            } else if (command == "neraser") {
                eraser_mode = false;
                draw_mode = true;
    
            } else if (command == "select") {
                select_mode = true;
                draw_mode = false;
                std::cout << "select_mode!\n";

            } else if (command == "nselect") {
                select_mode = false;
                draw_mode = true;

                // back to square one;
                sebox_fclick = 0;
                sebox_lclick = 0;
                seBox.setPos(0, 0);
                seBox.setSize(0, 0);
            }
        
        // after checking command, make NIL;
        command = NIL;


        sf::Vector2i pos = sf::Mouse::getPosition(window);
        eraser.setPos(pos.x, pos.y);

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


        window.clear(sf::Color::White);

        if (!play_mode) {
            window.draw(sprite);
            window.draw(&myVertices[0], myVertices.size(), sf::LineStrip);

            if (eraser_mode) {
                
                // draw a rectangle shape with cursor at midpoint
                sf::RectangleShape eraser_rect;
                int sz = eraser.getSize();
                eraser_rect.setSize(sf::Vector2f(sz, sz));
                eraser_rect.setOutlineColor(sf::Color::Black);
                eraser_rect.setOutlineThickness(1);
                eraser_rect.setPosition(eraser.getX(), eraser.getY());

                window.draw(eraser_rect);


            } else if (select_mode && sebox_fclick) {
                //sf::RectangleShape sebox_rect;
                seBox.sebox_rect.setSize(sf::Vector2f(seBox.getW(), seBox.getH()));
                seBox.sebox_rect.setOutlineColor(sf::Color::Black);
                seBox.sebox_rect.setOutlineThickness(3);
                seBox.sebox_rect.setPosition(seBox.getX(), seBox.getY());
                seBox.sebox_rect.setFillColor(sf::Color::Transparent); // trans

                if (DEBUG) {
                    std::cout << "draw srect w: " << seBox.getW();
                    std::cout << " h: " << seBox.getH();
                    std::cout << " x: " << seBox.getX();
                    std::cout << " y: " << seBox.getY() << std::endl;
                }

                window.draw(seBox.sebox_rect);
            }
                
            sprite.setTexture(cur_frame.layer_list[li].ltext);
            
        } else {
            
            /*
            std::cout << "playing animaton \n";

            // draw layers using sprites
            int i = 0;
            for (auto layer: cur_frame.layer_list) {
                mySprites[i].setTexture(layer.ltext);
                i++;
            }

            std::cout << "now drawing " << i << " sprites\n";

            int j = 0;


            for (j = 0; j < i; j++) {
                window.draw(mySprites[j]);
            }

            */


            std::cout << "Gonna naimato\n";

            sf::Image img1 = cur_frame.layer_list[0].ltext.copyToImage();
            sf::Image img2 = cur_frame.layer_list[1].ltext.copyToImage();

            sf::Image totalImg;
            totalImg.create(WIDTH, HEIGHT);
            getTotalImage(img1, img2, totalImg);

            sf::Texture t; // note: inefficient
            t.create(WIDTH, HEIGHT);
            t.update(totalImg);

            std::cout << "image getto\n!";

            sprite.setTexture(t);
            window.draw(sprite);

        }

        window.display();
    }

    return 7;

}
