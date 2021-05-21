#include "MainObject.h"


#define MAX_FALL_SPEED 10
#define GRAVITY_SPEED 0.8
#define PLAYER_SPEED 5

const int number_frame_x_ = 1;
const int number_frame_y_ = 1;

const int MAP_START = 0;
const int MAP_END = 0;

MainObject::MainObject()
{
    frame_ = 0;
    x_pos_ = 240;
    y_pos_ = 17*40;
    x_val_ = 0;
    y_val_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    status_ = -1;
    input_type_.right_ = 0;
    input_type_.left_ = 0;
    input_type_.up_ = 0;
    input_type_.down_ = 0;
    input_type_.jump_ = 0;
}

MainObject::~MainObject()
{

}

bool MainObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path,screen);

    if(ret == true)
    {
        width_frame_ = rect_.w/number_frame_x_;
        height_frame_ = rect_.h/number_frame_x_;
    }

    return ret;
}

SDL_Rect MainObject::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = rect_.w;
    rect.h = rect_.h;

    return rect;
}

void MainObject::set_clips()
{
    if(width_frame_ > 0 && height_frame_ > 0)
    {
        frame_clip_[0].x = 0;
        frame_clip_[0].y = 0;
        frame_clip_[0].w = width_frame_;
        frame_clip_[0].h = height_frame_;
    }
}

void MainObject::Show(SDL_Renderer* des)
{
    if(status_ == WALK_LEFT)
    {
        LoadImg("img//box.png",des);
    }
    else
    {
        LoadImg("img//box.png",des);
    }

    if(input_type_.left_ == 1 || input_type_.right_ == 1)
    {
        frame_ = 0;
    }
    else
    {
        frame_ = 0;
    }

    rect_.x = x_pos_;
    rect_.y = y_pos_;

    SDL_Rect* current_clip = &frame_clip_[frame_];

    SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};

    SDL_RenderCopy(des,p_object_, current_clip, &renderQuad);
}

void MainObject::HandelInputAction(SDL_Event events, SDL_Renderer* screen)
{
    if(events.type == SDL_KEYDOWN)
    {
        switch(events.key.keysym.sym)
        {
        case SDLK_RIGHT:
            {
                status_ = WALK_RIGHT;
                input_type_.right_ = 1;
                input_type_.left_ = 0;
            }
            break;
        case SDLK_LEFT:
            {
                status_ = WALK_LEFT;
                input_type_.left_ = 1;
                input_type_.right_ = 0;

            }
        break;
        }
    }
    else if(events.type == SDL_KEYUP)
    {
        switch(events.key.keysym.sym)
        {
        case SDLK_RIGHT:
            {
                input_type_.right_ = 0;

            }
            break;


        case SDLK_LEFT:
            {
                input_type_.left_ = 0;

            }
        break;
        }
    }
}

void MainObject::DoPlayer(Map& map_data)
{
    x_val_ = 0;
    y_val_ = 0;
    if(y_val_ >= MAX_FALL_SPEED)
    {
        y_val_ = MAX_FALL_SPEED;
    }

    if(input_type_.left_ == 1)
    {
        x_val_ -= PLAYER_SPEED;
    }
    else if(input_type_.right_ == 1)
    {
        x_val_ += PLAYER_SPEED;
    }
    CheckToMap(map_data);
}

void MainObject::CheckToMap(Map& map_data)
{
    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;

    int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;

    x_pos_ += x_val_;
    y_pos_ += y_val_;

    if(x_pos_ < 0 + MAP_START)
    {
        x_pos_ = 0 + MAP_START;
    }

    else if(x_pos_ + width_frame_ > map_data.max_x_ - MAP_END)
    {
        x_pos_ = map_data.max_x_ - width_frame_ - MAP_END;
    }

}


