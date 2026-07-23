#ifndef STRUCT_HPP
#define STRUCT_HPP

#include <raylib.h>
#include <string>
#include <vector>

#define CAM_ENGINE 0
#define CAM_PLAYER 1

 struct Asset{
  std::string name;
  Vector3 pos;
  Color color = WHITE; // no tint
  float x_scale, y_scale, z_scale;

  Vector3 box_size; // for now only boxes for collision
  bool collision = true;
  Model data;
};

struct AssetList{
  std::string name;
  int cnt; // for naming only (not the actual count)
  std::string file_loc;
};

struct Cam{
  Camera3D engine;
  Camera3D player;
  int selected;
};

struct EngineContext{
  std::vector<Asset> world_obj;
  std::vector<AssetList> assets_list; int selected_asset;

  Cam camera;
};

#endif
