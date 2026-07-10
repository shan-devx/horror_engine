#include "imgui.h"
#include "raylib.h"
#include <raymath.h>
#include <cstdint>
#include "gui.h"
#include <rlImGui.h>
#include <string>
#include <vector>

ImVec4 bgcolor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

ImGuiWindowFlags window_flags =
ImGuiWindowFlags_NoResize |
ImGuiWindowFlags_NoCollapse |
ImGuiWindowFlags_NoMove;

struct Asset{
  std::string name;
  Vector3 pos;
  Model data;
  Color color = WHITE; // no tint
//  float scale;
  float x_scale, y_scale, z_scale;
};

struct AssetList{
  std::string name;
  int cnt; // for naming only (not the actual count)
  std::string file_loc;
};

std::vector<Asset> world_obj;
std::vector<AssetList> assets_list;
void asset_init(){
  assets_list.push_back({"Cube", 0,"default"});
}

void assets_menu(){
  ImGui::SetNextWindowPos(ImVec2(0, GetScreenHeight()-200));
  ImGui::SetNextWindowSize(ImVec2(GetScreenWidth(), 200));
  ImGui::Begin("Assets", nullptr, window_flags);

  for(AssetList &i : assets_list){
    if(ImGui::Button(i.name.c_str())){
      if(i.file_loc == "default"){
        i.cnt++;
        std::string label = i.name + std::to_string(i.cnt);
        if(i.name == "Cube")
          world_obj.push_back({label.c_str(), {0,0,0}, 
              LoadModelFromMesh(GenMeshCube(1, 1, 1)), WHITE, 1.0f, 1.0f, 1.0f});
      }
    }
  }

  ImGui::End();
}

void properties_menu(){
  ImGui::SetNextWindowPos(ImVec2(0,0));
  ImGui::SetNextWindowSize(ImVec2(300, GetScreenHeight()-200));
  ImGui::Begin("Properties", nullptr, window_flags);

  for(Asset& i : world_obj){
    if(ImGui::Button(("x##" + i.name).c_str())){
      i.x_scale++;
    }
    if(ImGui::Button(("y##" + i.name).c_str())){
      i.y_scale++;
    }
    if(ImGui::Button(("z##" +i.name).c_str())){
      i.z_scale++;
    }
  }

  ImGui::End();
}

int vp_width, vp_height;
bool vp_focus;
RenderTexture2D vp_screen;
Camera3D camera;
void viewport_init(){
  vp_width = GetScreenWidth() - 300; 
  vp_height = GetScreenHeight()-200;
  vp_screen = LoadRenderTexture(vp_width, vp_height);
  vp_focus = false;

  camera.position = {5.0f, 5.0f, 5.0f};
  camera.target = {0.0f, 0.0f, 0.0f};
  camera.up = {0.0f, 1.0f, 0.0f};
  camera.fovy = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;
}
void viewport_menu(){
  // raylib
  BeginTextureMode(vp_screen);
  ClearBackground(Color{
    uint8_t(bgcolor.x * 255),
    uint8_t(bgcolor.y * 255),
    uint8_t(bgcolor.z * 255),
    uint8_t(bgcolor.w * 255),
  });
  BeginMode3D(camera);

  DrawGrid(10, 1);

  for(Asset& i : world_obj){
    Model model = i.data;
    model.transform = MatrixScale(i.x_scale, i.y_scale, i.z_scale);

    DrawModel(model, i.pos, 1, i.color);
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
    UpdateCamera(&camera, CAMERA_FREE);
    DisableCursor();
  }

  rlImGuiImageRenderTextureFit(&vp_screen, true);

  ImGui::End();
}
