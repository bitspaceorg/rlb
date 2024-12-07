#pragma once
#include "raylib.h"
#include "rlgl.h"

class CameraRay {
private:
  Camera3D camera;
  int camera_mode;
  float fov;

public:
  bool is_toggle_sniper;
  CameraRay(const Vector3 &initialPosition, const Vector3 &initialTarget,
            float initialFov = 45.0f, int projection = CAMERA_PERSPECTIVE,
            int mode = CAMERA_FREE) {
    camera = {0};
    is_toggle_sniper = 1;
    camera.position = initialPosition, camera.target = initialTarget,
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = initialFov;
    fov = initialFov;
    camera.projection = projection;
    camera_mode = mode;
  }

  Camera3D &get_camera() { return camera; }
  int get_camera_mode() { return camera_mode; }
  void toggle_sniper_cam() {
    if (is_toggle_sniper) {
      camera.fovy = 20.0f;
    } else {
      camera.fovy = fov;
    }
    is_toggle_sniper = !is_toggle_sniper;
  }
};
