#include <cstdint>
#include <imgui.h>
#include <raylib.h>
#include <imgui_impl_raylib.h>
#include "gui.hpp"

int main(){
  InitWindow(1280, 720, "Horror Engine");

  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImGui::StyleColorsDark();
  ImGui_ImplRaylib_Init();
    // change font

  ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = {0.1f, 0.1f, 0.1f, 0.1f};
  SetExitKey(KEY_NULL);
  ImVec4 bgcolor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

  ImGuiWindowFlags window_flags =
  ImGuiWindowFlags_NoResize |
  ImGuiWindowFlags_NoCollapse |
  ImGuiWindowFlags_NoMove;

  std::vector<Asset> world_obj;
  std::vector<AssetList> assets_list; int selected_asset = -1;

  EngineContext e = {bgcolor, window_flags, world_obj, assets_list, selected_asset};

  asset_init(e);
  viewport_init();

  while(!WindowShouldClose()){
    ImGui_ImplRaylib_ProcessEvents();
    ImGui_ImplRaylib_NewFrame();
    ImGui::NewFrame();

    viewport_menu(e);
    world_menu(e);
    assets_menu(e);
    properties_menu(e);

    ImGui::Render();
    BeginDrawing();
    ClearBackground(Color{
      uint8_t(e.bgcolor.x * 255),
      uint8_t(e.bgcolor.y * 255),
      uint8_t(e.bgcolor.z * 255),
      uint8_t(e.bgcolor.w * 255),
    });
    ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());
    EndDrawing();
  }
  
  ImGui_ImplRaylib_Shutdown();
  ImGui::DestroyContext();

  CloseWindow();

  return 0;
}
