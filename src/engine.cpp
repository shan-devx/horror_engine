#include "imgui.h"
#include "raylib.h"
#include <cstdint>
#include <raymath.h>
#include "engine.hpp"
#include "scripts/player.hpp"
#include <rlImGui.h>
#include <string>
#include <vector>
#include <persistence.hpp>
#include <engine_struct.hpp>

ImGuiWindowFlags window_flags =
  ImGuiWindowFlags_NoResize |
  ImGuiWindowFlags_NoCollapse |
  ImGuiWindowFlags_NoMove;

ImVec4 bgcolor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

void asset_init(EngineContext &e){
  e.assets_list.push_back({"Cube", 0,"default"});
  e.assets_list.push_back({"Terrain", 0, "default"});
}

void assets_menu(EngineContext &e){
  ImGui::SetNextWindowPos(ImVec2(300, GetScreenHeight()-200));
  ImGui::SetNextWindowSize(ImVec2(GetScreenWidth()-300, 200));
  ImGui::Begin("Assets", nullptr, window_flags);

  for(AssetList &i : e.assets_list){
    if(ImGui::Button(i.name.c_str())){
      if(i.file_loc == "default"){
        i.cnt++;
        std::string label = i.name + std::to_string(i.cnt);
        if(i.name == "Cube")
          e.world_obj.push_back((Asset){label.c_str(), {0,0,0}, 
              WHITE, 1.0f, 1.0f, 1.0f,
              {1.0f, 1.0f, 1.0f}});
        if(i.name == "Terrain")
          e.world_obj.push_back((Asset){label.c_str(), {0,-1,0},
              WHITE, 50.0f, 1.0f, 50.0f,
              {50.0f, 1.0f, 50.0f}});
        load_asset_model(e.world_obj);
      }
    }
  }   

  ImGui::End();
}

void world_menu(EngineContext &e){
  ImGui::SetNextWindowPos(ImVec2(0, GetScreenHeight()-200));
  ImGui::SetNextWindowSize(ImVec2(300, 200));
  ImGui::Begin("World", nullptr, window_flags);

  if(e.camera.selected == CAM_ENGINE){
    if(ImGui::Button("Play")){
      e.camera.selected = CAM_PLAYER;
    }
  }
  else{
    if(ImGui::Button("Stop")){
      e.camera.selected = CAM_ENGINE;
    }
  }
  ImGui::SameLine();
  if(ImGui::Button("save")){
    save(e.world_obj);
  }

  for(int i = 0; i < e.world_obj.size(); i++){
    if(ImGui::Button(e.world_obj[i].name.c_str())){
      e.selected_asset = i;
    }
  }

  ImGui::End();
}

void properties_menu(EngineContext &e){
  ImGui::SetNextWindowPos(ImVec2(0,0));
  ImGui::SetNextWindowSize(ImVec2(300, GetScreenHeight()-200));
  ImGui::Begin("Properties", nullptr, window_flags);

  if(e.selected_asset != -1){
    Asset &a = e.world_obj[e.selected_asset];
    ImGui::SeparatorText(a.name.c_str());
    
    ImGui::Text("Positon:");
    ImGui::DragFloat("x-axis", &a.pos.x);
    ImGui::DragFloat("y-axis", &a.pos.y);
    ImGui::DragFloat("z-axis", &a.pos.z);
      
    ImGui::Text("Color:");
    float col[4] = {(float)a.color.r/255, (float)a.color.g/255, (float)a.color.b/255, (float)a.color.a/255};
    ImGui::ColorEdit4("RGBA", col);
    a.color.r = col[0] * 255;
    a.color.g = col[1] * 255;
    a.color.b = col[2] * 255;
    a.color.a = col[3] * 255;

    ImGui::Text("scale:");
    ImGui::DragFloat("x", &a.x_scale);
    ImGui::DragFloat("y", &a.y_scale);
    ImGui::DragFloat("z", &a.z_scale);

    ImGui::Checkbox("Collision", &a.collision);

    if(ImGui::Button("Delete")){
      e.world_obj.erase(e.world_obj.begin() + e.selected_asset);
      e.selected_asset = -1;
    }
  }

  ImGui::End();
}

void camera_init(EngineContext &e, Player &p){
  Camera3D ecam;

  ecam.position = {5.0f, 5.0f, 5.0f};
  ecam.target = {0.0f, 0.0f, 0.0f};
  ecam.up = {0.0f, 1.0f, 0.0f};
  ecam.fovy = 45.0f;
  ecam.projection = CAMERA_PERSPECTIVE;
  e.camera.engine = ecam;

  player_cam_init(e, p);

  e.camera.selected = CAM_ENGINE;
}

int vp_width, vp_height;
bool vp_focus;
RenderTexture2D vp_screen;
void viewport_init(){
  vp_width = GetScreenWidth() - 300; 
  vp_height = GetScreenHeight()-200;
  vp_screen = LoadRenderTexture(vp_width, vp_height);
  vp_focus = false;
}
void viewport_menu(EngineContext &e, Player &p){
  // raylib
  BeginTextureMode(vp_screen);
  ClearBackground(Color{
    uint8_t(bgcolor.x * 255),
    uint8_t(bgcolor.y * 255),
    uint8_t(bgcolor.z * 255),
    uint8_t(bgcolor.w * 255),
  });
  if(e.camera.selected == CAM_ENGINE){
    BeginMode3D(e.camera.engine);
  }
  else if(e.camera.selected == CAM_PLAYER){
    if(vp_focus) player_cam(p, e);
    BeginMode3D(e.camera.player);
  }
  DrawGrid(100, 1);

  for(Asset& i : e.world_obj){
    Model model = i.data;
    model.transform = MatrixScale(i.x_scale, i.y_scale, i.z_scale);

    DrawModel(model, i.pos, 1, i.color);
    DrawModelWires(model, i.pos, 1, BLACK);
  } 

  EndMode3D();
  EndTextureMode();

  // imgui
  ImGui::SetNextWindowPos(ImVec2(300, 0));
  ImGui::SetNextWindowSize(ImVec2(vp_width, vp_height));
  ImGui::Begin("Viewport", nullptr,  window_flags);

  if(ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) vp_focus = true;
  if((ImGui::IsMouseClicked(ImGuiMouseButton_Right) ||IsKeyPressed(KEY_ESCAPE)) && vp_focus){vp_focus = false; EnableCursor();}
  if(vp_focus){
    if(e.camera.selected == CAM_ENGINE){
      UpdateCamera(&e.camera.engine, CAMERA_FREE);
    }
    else{
      UpdateCamera(&e.camera.player, CAMERA_FIRST_PERSON);
    }
    DisableCursor();
  }

  rlImGuiImageRenderTextureFit(&vp_screen, true);

  ImGui::End();
}
