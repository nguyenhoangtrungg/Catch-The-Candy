#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "CommonFunc.h"
#include "BaseObject.h"

#define MAX_TILES 10

class TileMat : public BaseObject
{
public:
    TileMat(){;}
    ~TileMat(){;}
};

class GameMap
{
public:
  GameMap();
  ~GameMap();

  void LoadMap(char* name);
  void DrawMap(SDL_Renderer* des);
  void LoadMapTiles(SDL_Renderer* screen);
  void SetMap(const Map& gMap) {game_map_ = gMap;}
  Map getMap() const {return game_map_;};
private:
  Map game_map_;
  TileMat tile_mat_[MAX_TILES];
  Input input_type_;
};

#endif // GAME_MAP_h
