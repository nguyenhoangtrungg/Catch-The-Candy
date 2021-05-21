#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <windows.h>

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

static Mix_Chunk* g_sound_choice = NULL;
static Mix_Music* g_sound_backmus = NULL;
static Mix_Chunk* g_sound_game_over = NULL;
static Mix_Chunk* g_sound_collision = NULL;
static Mix_Chunk* g_sound_eat = NULL;

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 800;
const int SCREEN_BPP = 32;

const int COLER_KEY_R = 167;
const int COLER_KEY_G = 175;
const int COLER_KEY_B = 180;

const int FONT_MENU_SIZE = 40;
const int FONT_END_SIZE = 62;

const int RENDER_DRAW_COLOR = 0xff;


#define TILE_SIZE 40

#define MAX_MAP_X 15
#define MAX_MAP_Y 20

typedef struct Map
{
    int strart_x_;
    int strart_y_;

    int max_x_;
    int max_y_;

    int tile[MAX_MAP_Y][MAX_MAP_X];
    char* file_name_;
};

typedef struct Input
{
    int left_;
    int right_;
    int up_;
    int down_;
    int jump_;
};

namespace SDLCommonFunc
{
    bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
    bool CheckForcusWithRect(const int& x, const int& y, const SDL_Rect& rect);
}

#endif // COMMON_FUNCTION_H_
