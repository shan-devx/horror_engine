#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "raylib.h"

#define player_body 0.5f

#define GRAVITY         32.0f
#define MAX_SPEED       20.0f
#define CROUCH_SPEED     5.0f
#define JUMP_FORCE      12.0f
#define MAX_ACCEL      80.0f
#define FRICTION         0.76f
#define AIR_DRAG         0.70f
#define CONTROL         15.0f
#define CROUCH_HEIGHT    0.0f
#define STAND_HEIGHT     1.0f
#define BOTTOM_HEIGHT    0.5f

#define SENSITIVITY 0.01f

struct Player{
  bool isGrounded;
  Vector3 position;
  Vector3 velocity;
  Vector3 dir;
  
  float headLerp=STAND_HEIGHT;

  Vector2 lookRotation;
  Vector2 lean;
  float headTimer;
  float walkLerp;
};

struct EngineContext;

void player_cam_init(EngineContext &e, Player &p);
void player_cam(Player &p, EngineContext &e);

void UpdateCameraFPS(Camera &camera, Player &p);
void UpdateBody(Player &p, EngineContext &e, char side, char forward, bool jumpPressed, bool crouchHold);

#endif
