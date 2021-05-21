
#include "CommonFunc.h"
#include "BaseObject.h"
#include "MainObject.h"
#include "game_map.h"
#include "ThreatsObject.h"
#include "TextObject.h"
#include "PlayerPower.h"

using namespace std;

BaseObject g_background;
BaseObject g_end;
BaseObject g_highscore;

TTF_Font* font_time = NULL;
TTF_Font* font_menu = NULL;
TTF_Font* font_end = NULL;

int Point_ = 0, live_ = 0;
vector<int> HighScoreList;

bool Init(){
    bool success = true;

    int ret = SDL_Init(SDL_INIT_VIDEO);

    ifstream f_in("ans.txt");
    int data_;
    while(f_in >> data_) HighScoreList.push_back(data_);

    if(ret < 0)
        return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("Candy",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH,SCREEN_HEIGHT,
                                SDL_WINDOW_SHOWN);
    if(g_window == NULL)
    {
            success = false;
    }
    else
    {
        g_screen = SDL_CreateRenderer(g_window,-1,SDL_RENDERER_ACCELERATED);
        if(g_screen == NULL)
            success = false;
        else
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if(!(IMG_Init(imgFlags) && imgFlags))
                success = false;
        }

        if(TTF_Init() == -1)
        {
            success = false;
        }

        font_time = TTF_OpenFont("font//font1.ttf",15);
        font_menu = TTF_OpenFont("font//font2.ttf",FONT_MENU_SIZE);
        font_end = TTF_OpenFont("font//font2.ttf",62);
        if(font_time == NULL)
        {
            success = false;
        }
    }

    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        success = false;
    }
    else
    {
        g_sound_backmus = Mix_LoadMUS("mus//theme.wav");

        g_sound_choice = Mix_LoadWAV("mus//beep_.wav");
        g_sound_game_over = Mix_LoadWAV("mus//game_over.wav");
        g_sound_collision = Mix_LoadWAV("mus//collision2.wav");
        g_sound_eat = Mix_LoadWAV("mus//eating_candy.wav");
    }

    return success;

}

bool LoadBackground()
{
    bool retEnd = g_end.LoadImg("img//endgame.png", g_screen);
    bool ret = g_background.LoadImg("img//bg.png", g_screen);
    if(ret == false || retEnd == false)
        return false;
    return true;

}

std::vector<ThreatsObject*> MakeThreadList()
{
    std::vector<ThreatsObject*> list_threats;

    ThreatsObject* threats_objs = new ThreatsObject[5];

    for(int i = 0; i < 5; i++)
    {
        ThreatsObject* p_threat = threats_objs + i;
        if(p_threat != NULL)
        {
            p_threat->LoadImg("img//threat_level_1.png",g_screen);
            p_threat->set_clips();
            p_threat->set_x_pos(40 + i*104);
            p_threat->set_y_pos(0); //i*25

            BulletObject* p_bullet = new BulletObject();
            p_threat->InitBullet(p_bullet, g_screen,Point_);

            list_threats.push_back(p_threat);
        }
    }

    return list_threats;
}

int loadMenu()
{
    bool retMenu = g_background.LoadImg("img//bgmenu.png", g_screen);
    if(retMenu == false) return 2;

    const int kMenuItemNum = 3;
    SDL_Rect pos_arr[kMenuItemNum];
    pos_arr[0].x = 250;
    pos_arr[0].y = 210;
    pos_arr[0].h = FONT_MENU_SIZE;
    pos_arr[0].w = FONT_MENU_SIZE*4-5;

    pos_arr[1].x = 250;
    pos_arr[1].y = 265;
    pos_arr[1].h = FONT_MENU_SIZE;
    pos_arr[1].w = FONT_MENU_SIZE*10-5;

    pos_arr[2].x = 250;
    pos_arr[2].y = 320;
    pos_arr[2].h = FONT_MENU_SIZE;
    pos_arr[2].w = FONT_MENU_SIZE*4-5;

    TextObject text_menu[kMenuItemNum];

    text_menu[0].SetText("Play");
    text_menu[0].SetColor(TextObject::YELLOW_TEXT);

    text_menu[1].SetText("High score");
    text_menu[1].SetColor(TextObject::YELLOW_TEXT);

    text_menu[2].SetText("Exit");
    text_menu[2].SetColor(TextObject::YELLOW_TEXT);

    bool selected[kMenuItemNum] = {0, 0, 0};
    int xm = 0;
    int ym = 0;
    SDL_Event m_event;

    while(1+1 == 2)
    {
        SDL_RenderClear(g_screen);
        g_background.Render(g_screen,NULL);

        for(int i = 0; i < kMenuItemNum; i++)
        {
            text_menu[i].LoadFromRenderText(font_menu, g_screen);
            text_menu[i].RenderText(g_screen,pos_arr[i].x,pos_arr[i].y);
        }

        SDL_RenderPresent(g_screen);

        while(SDL_PollEvent(&m_event))
        {
            switch (m_event.type)
            {
                case SDL_QUIT:
                    return 2;
                case SDL_MOUSEMOTION:
                {
                    xm = m_event.motion.x;
                    ym = m_event.motion.y;

                    for(int i = 0; i < kMenuItemNum; i++)
                    {
                        if(SDLCommonFunc::CheckForcusWithRect(xm,ym,pos_arr[i]))
                        {
                            if(selected[i] == false)
                            {
                                Mix_PlayChannel(-1,g_sound_choice,0);
                                selected[i] = 1;
                                text_menu[i].SetColor(TextObject::RED_TEXT);
                            }
                        }
                        else
                        {
                            if(selected[i] == true)
                            {
                                selected[i] = 0;
                                text_menu[i].SetColor(TextObject::YELLOW_TEXT);
                            }
                        }
                    }
                }
                break;

                case SDL_MOUSEBUTTONDOWN:
                {
                    xm = m_event.button.x;
                    ym = m_event.button.y;
                    for(int i = 0; i < kMenuItemNum; i++)
                    {
                        if(SDLCommonFunc::CheckForcusWithRect(xm,ym,pos_arr[i]))
                        {
                            Mix_PlayChannel(-1,g_sound_choice,0);
                            return i;
                        }
                    }
                }
                break;
                case SDL_KEYDOWN:
                    if(m_event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        return 2;
                    }
                default:
                    break;
            }
        }
        SDL_RenderPresent(g_screen);
    }
    return 2;
}

int highScore(int& ok_)
{
    bool retHighScore = g_background.LoadImg("img//highscore.png", g_screen);
    if(!retHighScore) return 0;

    const int kMenuItemNum = 3;
    SDL_Rect pos_arr[kMenuItemNum];
    pos_arr[0].x = 360;
    pos_arr[0].y = 730;
    pos_arr[0].h = 26;
    pos_arr[0].w = 26*4-5;

    TextObject text_score[HighScoreList.size()];
    int text_score_length[HighScoreList.size()];

    for(int i = 0; i < HighScoreList.size(); i++)
    {
        std::string str_score = std::to_string(HighScoreList[i]);
        text_score[i].SetText(str_score);
        text_score[i].SetColor(TextObject::RED_TEXT);
        text_score_length[i] = str_score.size()-1;
    }

    TextObject text_menu[kMenuItemNum];
    text_menu[0].SetText("Back");
    text_menu[0].SetColor(TextObject::YELLOW_TEXT);

    SDL_Event hs_event;
    bool selected[kMenuItemNum] = {0};
    int xm = 0;
    int ym = 0;

    while(ok_ == 1)
    {
        SDL_RenderClear(g_screen);
        g_background.Render(g_screen,NULL);

        for(int i = 0; i < kMenuItemNum; i++)
        {
            text_menu[i].LoadFromRenderText(font_menu, g_screen);
            text_menu[i].RenderText(g_screen,pos_arr[i].x,pos_arr[i].y);
        }

        for(int i = 0; i < HighScoreList.size(); i++)
        {
            int Distance = 20*text_score_length[i]/2;

            text_score[i].LoadFromRenderText(font_menu, g_screen);
            text_score[i].RenderText(g_screen,390 - Distance ,285+i*45);
        }

        SDL_RenderPresent(g_screen);

        while(SDL_PollEvent(&hs_event))
        {
            switch (hs_event.type)
            {
                case SDL_QUIT:
                    ok_ = 2;

                case SDL_MOUSEMOTION:
                {
                    xm = hs_event.motion.x;
                    ym = hs_event.motion.y;

                    for(int i = 0; i < kMenuItemNum; i++)
                    {
                        if(SDLCommonFunc::CheckForcusWithRect(xm,ym,pos_arr[i]))
                        {
                            if(selected[i] == false)
                            {
                                Mix_PlayChannel(-1,g_sound_choice,0);
                                selected[i] = 1;
                                text_menu[i].SetColor(TextObject::RED_TEXT);
                            }
                        }
                        else
                        {
                            if(selected[i] == true)
                            {
                                selected[i] = 0;
                                text_menu[i].SetColor(TextObject::YELLOW_TEXT);
                            }
                        }
                    }
                }
                break;

                case SDL_MOUSEBUTTONDOWN:
                {
                    xm = hs_event.button.x;
                    ym = hs_event.button.y;
                    for(int i = 0; i < kMenuItemNum; i++)
                    {
                        if(SDLCommonFunc::CheckForcusWithRect(xm,ym,pos_arr[i]))
                        {
                            Mix_PlayChannel(-1,g_sound_choice,0);
                            ok_ = 0;
                        }
                    }
                }
                break;

                case SDL_KEYDOWN:
                    if(hs_event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        ok_ = 2;
                    }
                default:
                    break;
            }
        }

    SDL_RenderPresent(g_screen);
    }
}

int checkEnd()
{

    const int kMenuItemNum = 1;
    SDL_Rect pos_arr[kMenuItemNum];
    pos_arr[0].x = 95;
    pos_arr[0].y = 460;
    pos_arr[0].h = FONT_END_SIZE;
    pos_arr[0].w = FONT_END_SIZE*13-5;

    TextObject text_menu[kMenuItemNum];

    text_menu[0].SetText("One more time");
    text_menu[0].SetColor(TextObject::RED_END_TEXT);

    bool selected[kMenuItemNum] = {0};
    int xm = 0;
    int ym = 0;
    SDL_Event m_event;

    while(1 + 1 != 3)
    {

        for(int i = 0; i < kMenuItemNum; i++)
        {
            text_menu[i].LoadFromRenderText(font_end, g_screen);
            text_menu[i].RenderText(g_screen,pos_arr[i].x,pos_arr[i].y);
        }

        SDL_RenderPresent(g_screen);

        while(SDL_PollEvent(&m_event))
        {
            switch (m_event.type)
            {
                case SDL_QUIT:
                    return 0;
                case SDL_MOUSEMOTION:
                {
                    xm = m_event.motion.x;
                    ym = m_event.motion.y;

                    for(int i = 0; i < kMenuItemNum; i++)
                    {
                        if(SDLCommonFunc::CheckForcusWithRect(xm,ym,pos_arr[i]))
                        {
                            if(selected[i] == false)
                            {
                                Mix_PlayChannel(-1,g_sound_choice,0);
                                selected[i] = 1;
                                text_menu[i].SetColor(TextObject::YELLOW_TEXT);
                            }
                        }
                        else
                        {
                            if(selected[i] == true)
                            {
                                selected[i] = 0;
                                text_menu[i].SetColor(TextObject::RED_END_TEXT);
                            }
                        }
                    }
                }
                break;

                case SDL_MOUSEBUTTONDOWN:
                {
                    xm = m_event.button.x;
                    ym = m_event.button.y;
                    for(int i = 0; i < kMenuItemNum; i++)
                    {
                        if(SDLCommonFunc::CheckForcusWithRect(xm,ym,pos_arr[i]))
                        {
                            Mix_PlayChannel(-1,g_sound_choice,0);
                            return 1;
                        }
                    }
                }
                break;
                case SDL_KEYDOWN:
                    if(m_event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        return 0;
                    }
                default:
                    break;
            }
        }
        SDL_RenderPresent(g_screen);
    }
    return 0;
}

void Export()
{
    int k, n = HighScoreList.size();
    k = Point_;

    if(n < 10 || k > HighScoreList[n-1])
    {
        for(int i = 0; i < n; i++)
            if(HighScoreList[i] < k)
            {
                HighScoreList.insert(HighScoreList.begin()+i,k);
                break;
            }
    }

    while(HighScoreList.size() > 10) HighScoreList.pop_back();
    ofstream f_out("ans.txt");
    for(int i = 0; i < HighScoreList.size(); i++) f_out << HighScoreList[i] << endl;
}

void close()
{
    Mix_FreeMusic(g_sound_backmus);

    Mix_FreeChunk(g_sound_choice);
    Mix_FreeChunk(g_sound_game_over);
    Mix_FreeChunk(g_sound_collision);
    Mix_FreeChunk(g_sound_eat);

    while(HighScoreList.size() > 0) HighScoreList.pop_back();
    g_background.Freee();

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    if(Init() == false)
        return -1;

    bool is_quit = false;
    int ok_  = 1;

    while(true)
    {
        ok_ = loadMenu();
        if(ok_ != 1) break;
        highScore(ok_);
    }

    if(ok_ == 2) is_quit = true;

    if(LoadBackground() == false)
        return -1;

    GameMap game_map;
    game_map.LoadMap("map//map01.txt");
    game_map.LoadMapTiles(g_screen);

    MainObject p_player;

    p_player.LoadImg("img//box.png",g_screen);
    p_player.set_clips();

    PlayerPower player_power;
    player_power.Init(g_screen);

    std::vector<ThreatsObject*> threats_list = MakeThreadList();

    TextObject time_game;
    time_game.SetColor(TextObject::RED_TEXT);

    while(!is_quit)
    {
        if( Mix_PlayingMusic() == 0 )
        {
            Mix_PlayMusic( g_sound_backmus, -1 );
        }

        while(SDL_PollEvent(&g_event) != 0)
        {
            if(g_event.type == SDL_QUIT)
            {
                is_quit = true;
            }

            p_player.HandelInputAction(g_event,g_screen);
        }

        SDL_RenderClear(g_screen);

        g_background.Render(g_screen,NULL);

        game_map.DrawMap(g_screen);
        Map map_data = game_map.getMap();

        p_player.DoPlayer(map_data);
        p_player.Show(g_screen);

        for(int i = 0; i < threats_list.size(); i++)
        {
            ThreatsObject* p_threat = threats_list.at(i);
            if(p_threat != NULL)
            {
                p_threat->SetMapXY(map_data.strart_x_, map_data.strart_y_);
                p_threat->DoPlayer(map_data);
                p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
                SDL_Delay(2);
                p_threat->Show(g_screen);

                SDL_Rect rect_player = p_player.GetRectFrame();
                bool bCol1 = false;
                std::vector<BulletObject*> tBullet_list = p_threat->get_bullet_list();
                for(int j = 0; j < tBullet_list.size(); j++)
                {
                    BulletObject* pt_bullet = tBullet_list.at(j);
                    if(pt_bullet != NULL)
                    {
                        SDL_Rect pt_rect = pt_bullet->GetRect();
                        bCol1 = SDLCommonFunc::CheckCollision(pt_rect, rect_player); //pt_bullet->GetRect()
                        if(bCol1) {
                            p_threat->RemoveBullet(j);
                            BulletObject* p_bullet = new BulletObject();
                            p_threat->InitBullet(p_bullet, g_screen,Point_);

                            Point_ += 5;

                            Mix_PlayChannel(-1,g_sound_eat,0);

                            break;
                        }
                        else if(pt_rect.y + pt_rect.h >= SCREEN_HEIGHT - 40)
                        {
                            Mix_PlayChannel(-1,g_sound_collision,0);

                            live_++;
                            player_power.Decrease();
                            player_power.Render(g_screen);

                            if(live_ == 4)
                            {
                                Mix_PauseMusic();
                                Mix_PlayChannel(-1,g_sound_game_over,0);
                                SDL_Delay(4200);

                                SDL_RenderClear(g_screen);
                                g_end.Render(g_screen,NULL);

                                Export();

                                std::string str_point = "";
                                std::string str_val = std::to_string(Point_);
                                str_point += str_val;

                                TextObject text_point;
                                text_point.SetColor(TextObject::RED_END_TEXT);
                                text_point.SetText(str_point);
                                text_point.LoadFromRenderText(font_end, g_screen);
                                text_point.RenderText(g_screen,SCREEN_WIDTH / 2 - 21*(str_point.size()) ,330);

                                SDL_RenderPresent(g_screen);
                                int checkEnd_ = checkEnd();
                                if(!checkEnd_) is_quit = 1;
                                for(int i = 1; i <= 5; i++) threats_list.pop_back();

                                for(int i = 0; i < 5; i++)
                                {
                                    ThreatsObject* pp_threat = new ThreatsObject;
                                    if(pp_threat != NULL)
                                    {
                                        pp_threat->LoadImg("img//threat_level_1.png",g_screen);
                                        pp_threat->set_clips();
                                        pp_threat->set_x_pos(40 + i*104);
                                        pp_threat->set_y_pos(0); //i*25

                                        BulletObject* pp_bullet = new BulletObject();
                                        pp_threat->InitBullet(pp_bullet, g_screen,Point_);

                                        threats_list.push_back(pp_threat);
                                    }
                                }

                                Mix_ResumeMusic();

                                Point_ = 0;
                                live_ = 0;
                                player_power.Init(g_screen);
                            }

                        }
                    }
                }
            }
        }

        player_power.Show(g_screen);

        std::string str_score = "Score:";
        std::string str_val = std::to_string(Point_);
        str_score += str_val;

        time_game.SetText(str_score);
        time_game.LoadFromRenderText(font_time, g_screen);
        time_game.RenderText(g_screen,SCREEN_WIDTH - 140,60);

        SDL_RenderPresent(g_screen);
    }


    close();

    return 0;
}
