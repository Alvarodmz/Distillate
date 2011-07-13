#include <iostream>
#include <stdexcept>
#include <vector>
#include "Distillate.hpp"

using namespace Distillate;

class State : public DState
{
public:
    DSprite *player;

    State() : DState() {};
    void update() 
    {
        DState::update();
        player->velocity->x = 0;     
        if(DGlobals::keys->checkKeyState(SDL_KEYUP, SDLK_ESCAPE))
            DGlobals::quit();

        if(DGlobals::keys->checkKeyState(SDL_KEYDOWN, SDLK_RIGHT))
            player->velocity->x += 100;
    }

    void create() 
    {
        player = new DSprite(10,10);
        player->loadGraphic("player.png", true, false, 52, 21, false);

        std::vector<int> anim_frame;
        anim_frame.push_back(0);
        anim_frame.push_back(1);
        anim_frame.push_back(2);

        player->addAnimation("flying", anim_frame, 3, true);
        player->play("flying");


        add(player);
    }
};

class Test : public DGame
{
public:    
    Test() : DGame(640, 480, new State()) {};
};


int main(int argc, char* argv[])
{
    try
    {
        Test* test = new Test();
        delete test;
    }
    catch(std::exception &e)
    {
        std::cout << "Ops: " << e.what() << std::endl;
    }

    return 0;
}
