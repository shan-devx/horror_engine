#include "raylib.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <engine_struct.hpp>

using json = nlohmann::json;

void to_json(json &j, const Vector3 &v){
  j = {
    {"x", v.x},
    {"y", v.y},
    {"z", v.z},
  };
}

void from_json(const json &j ,Vector3 &v){
  j.at("x").get_to(v.x);
  j.at("y").get_to(v.y);
  j.at("z").get_to(v.z);
}

void to_json(json &j, const Color &c){
  j = {
    {"r", c.r},
    {"g", c.g},
    {"b", c.b},
    {"a", c.a},
  };
}

void from_json(const json &j, Color &c){
  j.at("r").get_to(c.r);
  j.at("g").get_to(c.g);
  j.at("b").get_to(c.b);
  j.at("a").get_to(c.a);
}

void to_json(json &j, const Asset &a){
  j = {
    {"name", a.name},
    {"pos", a.pos},
    {"color", a.color},
    {"x_scale", a.x_scale},
    {"y_scale", a.y_scale},
    {"z_scale", a.z_scale},
    {"box_size", a.box_size},
    {"collision", a.collision},
  };
}

void from_json(const json &j, Asset &a){
  j.at("name").get_to(a.name);
  j.at("pos").get_to(a.pos);
  j.at("color").get_to(a.color);
  j.at("x_scale").get_to(a.x_scale);
  j.at("y_scale").get_to(a.y_scale);
  j.at("z_scale").get_to(a.z_scale);
  j.at("box_size").get_to(a.box_size);
  j.at("collision").get_to(a.collision);
}

void save(std::vector<Asset> &world_obj){
  json j = world_obj;

  std::ofstream file("scene.json");
  file << j.dump(2);
}

std::vector<Asset> load(){
  std::ifstream file("scene.json");

  json j;
  file >> j;

  return j.get<std::vector<Asset>>();
}


void load_asset_model(std::vector<Asset> &world_obj){
  for(Asset &i : world_obj){
    if(i.name.starts_with("Cube")){
      i.data = LoadModelFromMesh(GenMeshCube(1, 1, 1));
    }
    if(i.name.starts_with("Terrain")){
      i.data = LoadModelFromMesh(GenMeshCube(1, 1, 1));
    }
  }
}
