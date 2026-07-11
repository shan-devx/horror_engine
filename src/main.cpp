#include <cstdint>
#include <imgui.h>
#include <raylib.h>
#include <imgui_impl_raylib.h>
#include "gui.h"

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

  asset_init();
  viewport_init();

  while(!WindowShouldClose()){
    ImGui_ImplRaylib_ProcessEvents();
    ImGui_ImplRaylib_NewFrame();
    ImGui::NewFrame();

    viewport_menu();
    world_menu();
    assets_menu();
    properties_menu();

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
