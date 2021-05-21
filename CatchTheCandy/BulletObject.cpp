#include "BulletObject.h"


BulletObject::BulletObject()
{
    x_val_ = 0;
    y_val_ = 0;
    count_ = 0;
    valtoend_ = rand() % 500;
    is_move_ = false;
}


BulletObject::~BulletObject()
{

}

bool BulletObject::LoadImgBullet(SDL_Renderer* des)
{
    bool ret = false;
    ret = LoadImg("img//candy.png", des);
    return ret;
}

void BulletObject::HandleMove(const int& x_border, const int& y_border)
{
    if (bullet_dir_ == DIR_DOWN)
    {
        count_++;
        rect_.y += y_val_;
        if(count_ < valtoend_) rect_.y -= y_val_;;
        if (rect_.y > y_border - 100 - 1)
        {
            count_ = 0;
            valtoend_ = rand() % 300;
            //valtoend_--;
            is_move_ = false;
        }
    }
}
