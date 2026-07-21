#ifndef GUI_H
#define GUI_H

#include "imgui.h"
#include "raylib.h"
#include <string>
#include <vector>

#define CAM_ENGINE 0
#define CAM_PLAYER 1

 struct Asset{
  std::string name;
  Vector3 pos;
  Model data;
  Color color = WHITE; // no tint
  float x_scale, y_scale, z_scale;

  Vector3 box_size; // for now only boxes for collision
  bool collision = true;
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
  ImVec4 bgcolor;
  
  ImGuiWindowFlags wflags;

  std::vector<Asset> world_obj;
  std::vector<AssetList> assets_list; int selected_asset;

  Cam camera;
};

struct Player;

void camera_init(EngineContext &e, Player &p);
void asset_init(EngineContext &e);
void viewport_init();
void viewport_menu(EngineContext &e, Player &p);
void assets_menu(EngineContext &e);
void world_menu(EngineContext &e);
void properties_menu(EngineContext &e);

#endif
