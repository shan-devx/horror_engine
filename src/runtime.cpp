#include <raylib.h>
#include <raymath.h>
#include <scripts/player.hpp>
#include <persistence.hpp>
#include <engine_struct.hpp>

#define GAME_NAME "game"
#define WIDTH 1280
#define HEIGHT 720

int main(){
  Color bgcolor = {51, 51, 51, 255};
  InitWindow(WIDTH, HEIGHT, GAME_NAME);
  SetTargetFPS(60);

  DisableCursor();

  Player p = {};
  EngineContext e;
  e.world_obj = load();
  load_asset_model(e.world_obj);

  player_cam_init(e, p);

  while(!WindowShouldClose()){
    player_cam(p, e);

    BeginDrawing();
    ClearBackground(bgcolor);

    BeginMode3D(e.camera.player);

    for(Asset& i : e.world_obj){
      i.data.transform = MatrixScale(i.x_scale, i.y_scale, i.z_scale);

      DrawModel(i.data, i.pos, 1, i.color);
      DrawModelWires(i.data, i.pos, 1, BLACK);
    }

      EndMode3D();
      EndDrawing();
    }

  CloseWindow();
  return 0;
}
