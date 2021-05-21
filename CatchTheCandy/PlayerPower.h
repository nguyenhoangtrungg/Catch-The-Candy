#ifndef PLAYER_POWER_H_
#define PLAYER_POWER_H_

#include "CommonFunc.h"
#include "BaseObject.h"

class PlayerPower : public BaseObject
{
public:
    PlayerPower();
    ~PlayerPower();

    void SetNum(const int& num) {number_ = num; };
    void AddXPos(const int& xPos);
    void AddYPos(const int& yPos);
    void Show(SDL_Renderer* screen);
    void Init(SDL_Renderer* screen);

    void Decrease();

private:
    int number_;
    std::vector<int> pos_list_;

};

#endif // PLAYER_POWER_H_
