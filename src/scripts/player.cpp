#include "scripts/player.hpp"
#include "raylib.h"
#include "raymath.h"
#include "engine_struct.hpp"

void player_cam_init(EngineContext &e, Player &p){
  e.camera.player.position = {p.position.x, 
    p.position.y + player_body + p.headLerp, p.position.z};
  e.camera.player.target = {1.0f, 2.0f, 1.0f};
  e.camera.player.up = {0.0f, 1.0f, 0.0f};
  e.camera.player.fovy = 45.0f;
  e.camera.player.projection = CAMERA_PERSPECTIVE;
  UpdateCameraFPS(e.camera.player, p);
}

void player_cam(Player &p, EngineContext &e){
      Vector2 mouseDelta = GetMouseDelta();
      p.lookRotation.x -= mouseDelta.x * SENSITIVITY;
      p.lookRotation.y += mouseDelta.y * SENSITIVITY;

      char sideway = (IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
      char forward = (IsKeyDown(KEY_W) - IsKeyDown(KEY_S));
      bool crouching = IsKeyDown(KEY_LEFT_CONTROL);

      UpdateBody(p, e, sideway, forward, IsKeyPressed(KEY_SPACE), crouching);

      float delta = GetFrameTime();
      p.headLerp = Lerp(p.headLerp, (crouching ? CROUCH_HEIGHT : STAND_HEIGHT), 20.0f*delta);
      e.camera.player.position = {p.position.x,
                                  p.position.y + (BOTTOM_HEIGHT + p.headLerp),
                                  p.position.z,
                                  };

      if(p.isGrounded && ((forward != 0) || (sideway != 0))){
        p.headTimer += delta*3.0f;
        p.walkLerp = Lerp(p.walkLerp, 1.0f, 10.0f*delta);
        e.camera.player.fovy = Lerp(e.camera.player.fovy, 55.0f, 5.0f*delta);
      }
      else{
        p.walkLerp = Lerp(p.walkLerp, 0.0f, 10.0f*delta);
        e.camera.player.fovy = Lerp(e.camera.player.fovy, 60.0f, 5.0f*delta);
      }
      p.lean.x = Lerp(p.lean.x, sideway*0.02f, 10.0f*delta);
      p.lean.y = Lerp(p.lean.y, forward*0.015f, 10.0f*delta);

      UpdateCameraFPS(e.camera.player, p);
}

bool is_collision(Player &p, EngineContext &e);
// raylib-src/core_3d_camera_fps eg
void UpdateCameraFPS(Camera &camera, Player &p){
  const Vector3 up = { 0.0f, 1.0f, 0.0f };
  const Vector3 targetOffset = { 0.0f, 0.0f, -1.0f };

  Vector3 yaw = Vector3RotateByAxisAngle(targetOffset, up, p.lookRotation.x);

  float maxAngleUp = Vector3Angle(up, yaw);
  maxAngleUp -= 0.001f; // Avoid numerical errors
  if ( -(p.lookRotation.y) > maxAngleUp) { p.lookRotation.y = -maxAngleUp; }

  float maxAngleDown = Vector3Angle(Vector3Negate(up), yaw);
  maxAngleDown *= -1.0f; // Downwards angle is negative
  maxAngleDown += 0.001f; // Avoid numerical errors
  if ( -(p.lookRotation.y) < maxAngleDown) { p.lookRotation.y = -maxAngleDown; }

  Vector3 right = Vector3Normalize(Vector3CrossProduct(yaw, up));

  float pitchAngle = -p.lookRotation.y - p.lean.y;
  pitchAngle = Clamp(pitchAngle, -PI/2 + 0.0001f, PI/2 - 0.0001f); // Clamp angle so it doesn't go past straight up or straight down
  Vector3 pitch = Vector3RotateByAxisAngle(yaw, right, pitchAngle);

  // Head animation
  // Rotate up direction around forward axis
  float headSin = sinf(p.headTimer*PI);
  float headCos = cosf(p.headTimer*PI);
  const float stepRotation = 0.01f;
  camera.up = Vector3RotateByAxisAngle(up, pitch, headSin*stepRotation + p.lean.x);

  // Camera BOB
  const float bobSide = 0.1f;
  const float bobUp = 0.15f;
  Vector3 bobbing = Vector3Scale(right, headSin*bobSide);
  bobbing.y = fabsf(headCos*bobUp);

  camera.position = Vector3Add(camera.position, Vector3Scale(bobbing, p.walkLerp));
  camera.target = Vector3Add(camera.position, pitch);
}

// raylib-src/core_3d_camera_fps eg
void UpdateBody(Player &p, EngineContext &e, char side, char forward, bool jumpPressed, bool crouchHold){

  float rot = p.lookRotation.x;
  Vector2 input = (Vector2){ (float)side, (float)-forward };
  if ((side != 0) && (forward != 0)) input = Vector2Normalize(input);

  float delta = GetFrameTime();

  if (!p.isGrounded) p.velocity.y -= GRAVITY*delta;

  if (p.isGrounded && jumpPressed){
    p.velocity.y = JUMP_FORCE;
    p.isGrounded = false;
  }

  Vector3 front = (Vector3){ sinf(rot), 0.f, cosf(rot) };
  Vector3 right = (Vector3){ cosf(-rot), 0.f, sinf(-rot) };

  Vector3 desiredDir = (Vector3){ input.x*right.x + input.y*front.x, 0.0f, input.x*right.z + input.y*front.z, };
  p.dir = Vector3Lerp(p.dir, desiredDir, CONTROL*delta);

    float decel = (p.isGrounded ? FRICTION : AIR_DRAG);
    Vector3 hvel = (Vector3){ p.velocity.x*decel, 0.0f, p.velocity.z*decel };

    float hvelLength = Vector3Length(hvel); // Magnitude
    if (hvelLength < (MAX_SPEED*0.01f)) hvel = (Vector3){ 0 };

    // This is what creates strafing
    float speed = Vector3DotProduct(hvel, p.dir);

    // Whenever the amount of acceleration to add is clamped by the maximum acceleration constant,
    // a Player can make the speed faster by bringing the direction closer to horizontal velocity angle
    float maxSpeed = (crouchHold? CROUCH_SPEED : MAX_SPEED);
    float accel = Clamp(maxSpeed - speed, 0.f, MAX_ACCEL*delta);
    hvel.x += p.dir.x*accel;
    hvel.z += p.dir.z*accel;

    p.velocity.x = hvel.x;
    p.velocity.z = hvel.z;

    p.position.x += p.velocity.x*delta;
    if(is_collision(p, e)){ p.position.x -= p.velocity.x*delta; p.velocity.x = 0;}

    p.position.y += p.velocity.y*delta;
    if(is_collision(p, e)){
      p.position.y-=p.velocity.y*delta;
      if(p.velocity.y < 0){
        p.isGrounded = true;
      }
      p.velocity.y = 0;
    }
    else{
      p.isGrounded = false;
    }

    p.position.z += p.velocity.z*delta;
    if(is_collision(p, e)){ p.position.z -= p.velocity.z*delta; p.velocity.z = 0;}
}

bool is_collision(Player &p, EngineContext &e){
    BoundingBox player_box = {(Vector3){p.position.x-0.5f,
                                        p.position.y,
                                        p.position.z-0.5f,},
                              (Vector3){p.position.x+0.5f,
                                        p.position.y + player_body + p.headLerp,
                                        p.position.z+0.5f}
                              };

    for(Asset &i : e.world_obj){
      if(!i.collision) continue;
      BoundingBox obj_box = {(Vector3){i.pos.x - i.box_size.x/2,
                                       i.pos.y - i.box_size.y/2,
                                       i.pos.z - i.box_size.z/2,},
                             (Vector3){i.pos.x + i.box_size.x/2,
                                       i.pos.y + i.box_size.y/2,
                                       i.pos.z + i.box_size.z/2,}
                            };
      if(CheckCollisionBoxes(player_box, obj_box)){
        return true;
      }
    }

    return false;
}
