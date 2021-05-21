
#ifndef THREATS_OBJECT_H_
#define THREATS_OBJECT_H_

#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"

#define THREAT_FRAME_NUM 8
#define TH_GRAVITY_SPEED 0.8
#define TH_MAX_FALL_SPEED 1
#define TH_MAP_START 0
#define TH_MAP_END 0

class ThreatsObject : public BaseObject
{
public:
    ThreatsObject();
    ~ThreatsObject();

    void set_x_val(const float& xVal){x_val_ = xVal;}
    void set_y_val(const float& yVal){y_val_ = yVal;}

    void set_x_pos(const float& xp){x_pos_ = xp;}
    void set_y_pos(const float& yp){y_pos_ = yp;}
    float get_x_pos() const {return x_pos_;}
    float get_y_pos() const {return y_pos_;}
    void SetMapXY(const int& mp_x, const int& mp_y){map_x_ = mp_x, map_y_ = mp_y;}

    void set_clips();
    bool LoadImg(std::string path, SDL_Renderer* screen);
    void Show(SDL_Renderer* des);
    int get_width_frame() const {return width_frame_;}
    int get_height_frame() const {return height_frame_;}
    void DoPlayer(Map& gMap);
    void CheckToMap(Map& gMap);

    std::vector<BulletObject*> get_bullet_list() const {return bullet_list;}
    void set_bullet_list(const std::vector<BulletObject*>& bl_list){bullet_list = bl_list;}

    void InitBullet(BulletObject* p_bullet, SDL_Renderer* screen, int point_);
    void MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit);
    void RemoveBullet(const int& idx);

    SDL_Rect GetRectFrame();

private:

    int map_x_;
    int map_y_;
    float x_val_;
    float y_val_;
    float x_pos_;
    float y_pos_;
    bool on_ground_;
    int come_back_time_;
    SDL_Rect frame_clip_[THREAT_FRAME_NUM];
    int width_frame_;
    int height_frame_;
    int frame_;

    std::vector<BulletObject*> bullet_list;
};

#endif // THREATS_OBJECT_H_
