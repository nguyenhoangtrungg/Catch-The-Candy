
#include "ThreatsObject.h"

ThreatsObject::ThreatsObject()
{
    width_frame_ = 0;
    height_frame_ = 0;
    x_val_ = 0.0;
    y_val_ = 0.0;
    x_pos_ = 0.0;
    y_pos_ = 0.0;
    on_ground_ = 0;
    come_back_time_ = 0;
    frame_ = 0;
}

ThreatsObject::~ThreatsObject()
{

}

bool ThreatsObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path,screen);
    if(ret)
    {
        width_frame_ = rect_.w;//THREAT_FRAME_NUM;
        height_frame_ = rect_.h;
    }

    return ret;
}

void ThreatsObject::set_clips()
{
    if(width_frame_ > 0 && height_frame_ > 0)
    {
        frame_clip_[0].x = 0;
        frame_clip_[0].y = 0;
        frame_clip_[0].w = width_frame_;
        frame_clip_[0].h = height_frame_;
    }
}

void ThreatsObject::Show(SDL_Renderer* des)
{
    if(come_back_time_ == 0)
    {
        rect_.x = x_pos_ - map_x_;
        rect_.y = y_pos_ - map_y_;

        SDL_Rect* currenClips = &frame_clip_[frame_];
        SDL_Rect rendQuad = {rect_.x, rect_.y, width_frame_, height_frame_};
        SDL_RenderCopy(des,p_object_,currenClips, &rendQuad);
    }
}

void ThreatsObject::DoPlayer(Map& gMap)
{
    if(come_back_time_ < 0)
    {
        x_val_ = 0;
        y_val_ += TH_GRAVITY_SPEED;
        if(y_val_ >= TH_MAX_FALL_SPEED)
        {
            y_val_ = TH_MAX_FALL_SPEED;
        }

        CheckToMap(gMap);
    }
    else if(come_back_time_ > 0)
    {
        come_back_time_--;

    }
}

void ThreatsObject::CheckToMap(Map& map_data)
{
    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;

    //Ktra chieu ngang
    int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;

    x_pos_ += x_val_;
    y_pos_ += y_val_;

    if(x_pos_ < 0 + TH_MAP_START)
    {
        x_pos_ = 0 + TH_MAP_START;
    }

    else if(x_pos_ + width_frame_ > map_data.max_x_ - TH_MAP_END)
    {
        x_pos_ = map_data.max_x_ - width_frame_ - TH_MAP_END;
    }

}

void ThreatsObject::InitBullet(BulletObject* p_bullet, SDL_Renderer* screen, int point_)
{
    //srand(time(NULL));
    if(p_bullet != NULL)
    {
        p_bullet->LoadImgBullet(screen);
        p_bullet->set_is_move(true);
        p_bullet->set_bullet_dir(BulletObject::DIR_DOWN);
        p_bullet->SetRect(x_pos_ + 20, y_pos_ + 10);
        p_bullet->set_y_val(1 + point_/100);
        bullet_list.push_back(p_bullet);
    }
}

void ThreatsObject::MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit)
{

    for(int i = 0; i < bullet_list.size(); i++)
    {
        int res = (int(x_pos_) + rand()) % 120;
        res = 0;
        BulletObject* p_bullet = bullet_list.at(i);
        if(p_bullet != NULL)
        {
            if(p_bullet->get_is_move())
            {
                p_bullet->HandleMove(x_limit,y_limit);
                p_bullet->Render(screen);
            }
            else
            {
                p_bullet->set_is_move(true);
                p_bullet->SetRect(x_pos_ + 20, res);
            }
        }
    }
}

void ThreatsObject::RemoveBullet(const int& idx)
{
    int size_ = bullet_list.size();
    if(size_ > 0 && idx < size_)
    {
        BulletObject* p_bullet = bullet_list.at(idx);
        bullet_list.erase(bullet_list.begin() + idx);

        if(p_bullet)
        {
            delete p_bullet;
            p_bullet = NULL;
        }
    }
}

SDL_Rect ThreatsObject::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;

    return rect;
}
