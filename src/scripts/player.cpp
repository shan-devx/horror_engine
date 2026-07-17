#include "scripts/player.hpp"
#include "imgui.h"
#include "raylib.h"
#include "raymath.h"
#include "engine.hpp"
#include <string>

void player_cam_init(EngineContext &e, Player &p){

  Camera3D pcam = {};

  pcam.position = {p.position.x, 
    p.position.y + player_body + p.headLerp, p.position.z};
//  pcam.position = {0, 3, 0};
  pcam.target = {1.0f, 2.0f, 1.0f};
  pcam.up = {0.0f, 1.0f, 0.0f};
  pcam.fovy = 60.0f;
  pcam.projection = CAMERA_PERSPECTIVE;
  e.camera.player = pcam;

  UpdateCameraFPS(pcam, p);
}

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
void UpdateBody(Player &p, char side, char forward, bool jumpPressed, bool crouchHold){

  float rot = p.lookRotation.x;
  Vector2 input = (Vector2){ (float)side, (float)-forward };

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
    p.position.y += p.velocity.y*delta;
    p.position.z += p.velocity.z*delta;

    // Fancy collision system against the floor
    if (p.position.y <= 0.0f){
        p.position.y = 0.0f;
        p.velocity.y = 0.0f;
        p.isGrounded = true; // Enable jumping
    }
}
