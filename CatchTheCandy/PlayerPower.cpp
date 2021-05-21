
#include "PlayerPower.h"

PlayerPower::PlayerPower()
{
    number_ = 0;
}

PlayerPower::~PlayerPower()
{

}

void PlayerPower::AddXPos(const int& xp)
{
    pos_list_.push_back(xp);
}

void PlayerPower::Init(SDL_Renderer* screen)
{
    LoadImg("img//coin.png",screen);
    number_ = 3;
    if(pos_list_.size() > 0)
    {
        pos_list_.clear();
    }

    AddXPos(25);
    AddXPos(60);
    AddXPos(95);
}

void PlayerPower::Show(SDL_Renderer* screen)
{
    for(int i = 0; i < pos_list_.size(); i++)
    {
        rect_.x = pos_list_.at(i);
        rect_.y = 50;
        Render(screen);
    }
}

void PlayerPower::Decrease()
{
    number_--;
    pos_list_.pop_back();
}
