#include <cstdint>
#include <imgui.h>
#include <raylib.h>
#include <imgui_impl_raylib.h>

ImVec4 bgcolor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

int main(){
  InitWindow(1360, 768, "Horror Engine");

  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImGui::StyleColorsDark();
  ImGui_ImplRaylib_Init();
    // change font
  

  while(!WindowShouldClose()){
    ImGui_ImplRaylib_ProcessEvents();
    ImGui_ImplRaylib_NewFrame();
    ImGui::NewFrame();



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
