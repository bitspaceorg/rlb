#pragma once
#include "raylib.h"
#include "rlgl.h"
#include <math.h>

struct Target {
  bool is_target_cam;
  Vector3 target_pos;
};

class CameraRay {
private:
  Camera3D camera;
  int camera_mode;
  float fov;
  Target target;

public:
  bool is_toggle_sniper;
  Vector3 prev_pos = Vector3{0.0f, 0.0f, 0.0f};
  CameraRay(const Vector3 &initialPosition = Vector3{0.0f, 0.0f, 0.0f},
            const Vector3 &initialTarget = Vector3{0.0f, 0.0f, 0.0f},
            float initialFov = 45.0f, int projection = CAMERA_PERSPECTIVE,
            int mode = CAMERA_FREE, bool initial_is_target_cam = 0) {
    camera = {0};
    is_toggle_sniper = 1;
    camera.position = initialPosition, camera.target = initialTarget,
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = initialFov;
    fov = initialFov;
    camera.projection = projection;
    camera_mode = mode;
    target.is_target_cam = initial_is_target_cam;
    target.target_pos = initialTarget;
  }

  Camera3D &get_camera() { return camera; }
  int get_camera_mode() { return camera_mode; }
  void toggle_sniper_cam(bool enable) {
    if (!enable) {
      camera.fovy = 20.0f;
    } else {
      camera.fovy = fov;
    }
    is_toggle_sniper = enable;
  }
  void set_fov(const float &fovy) {
    camera.fovy = fovy;
    fov = fovy;
  }
  float get_fov() { return fov; }
  Vector3 get_position() { return camera.position; }
  Vector3 get_target() { return camera.target; }
  Target get_target_struct() { return target; }
  int get_mode() { return camera_mode; }
  void set_mode(const int &mode) { camera_mode = mode; }

  BoundingBox get_bounding_box() {
    float half_width = tan(camera.fovy / 2) * camera.position.z;
    return {.min = {camera.position.x - half_width,
                    camera.position.y - half_width, camera.position.z - 0.1f},
            .max = {camera.position.x + half_width,
                    camera.position.y + half_width, camera.position.z + 0.1f}};
  }
};
