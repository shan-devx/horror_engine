#include <cstdint>
#include <fstream>
#include <imgui.h>
#include <raylib.h>
#include <imgui_impl_raylib.h>
#include "scripts/player.hpp"
#include <persistence.hpp>
#include <engine_struct.hpp>

int main(){
  ImVec4 bgcolor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

  InitWindow(1280, 720, "Horror Engine");

  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImGui::StyleColorsDark();
  ImGui_ImplRaylib_Init();
    // change font

  ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = {0.1f, 0.1f, 0.1f, 0.1f};
  SetExitKey(KEY_NULL);
  std::vector<Asset> world_obj;
  std::vector<AssetList> assets_list; int selected_asset = -1;
  EngineContext e = {world_obj, assets_list, selected_asset};
  
  std::ifstream file("scene.json");
  if(file.is_open()){
    e.world_obj = load();
    load_asset_model(e.world_obj);
  }
  Player p={};

  camera_init(e, p);
  asset_init(e);
  viewport_init();

  SetTargetFPS(60);

  while(!WindowShouldClose()){
    ImGui_ImplRaylib_ProcessEvents();
    ImGui_ImplRaylib_NewFrame();
    ImGui::NewFrame();

    viewport_menu(e, p);
    world_menu(e);
    assets_menu(e);
    properties_menu(e);

    ImGui::Render();
    BeginDrawing();
    ClearBackground(Color{
      uint8_t(bgcolor.x * 255),
      uint8_t(bgcolor.y * 255),
      uint8_t(bgcolor.z * 255),
      uint8_t(bgcolor.w * 255),
    });
    ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());
    EndDrawing();
  }
  
  ImGui_ImplRaylib_Shutdown();
  ImGui::DestroyContext();

  CloseWindow();

  return 0;
}
