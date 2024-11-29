#include "raylibwrapper.hpp"
#include "raylib.h"
#include "rlgl.h"
#include <cmath>

RaylibWrapper::RaylibWrapper(int width, int height, const std::string &title)
    : window_width(width), window_height(height), window_title(title) {
  camera = {0};
  camera.position = (Vector3){10.0f, 10.0f, 10.0f};
  camera.target = (Vector3){2.5f, 0.0f, 2.5f};
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  camera.fovy = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;
  camera_mode = CAMERA_FREE;
}

RaylibWrapper::~RaylibWrapper() { CloseWindow(); }

void RaylibWrapper::init() {
  InitWindow(window_width, window_height, window_title.c_str());
  SetTargetFPS(60);
}

void RaylibWrapper::update_camera() { UpdateCamera(&camera, camera_mode); }

float RaylibWrapper::distance(float x1, float y1, float x2, float y2) {
  return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

float RaylibWrapper::angle_between_points(float p1x, float p1y, float p2x,
                                          float p2y) {
  float deltaY = p2y - p1y;
  float deltaX = p2x - p1x;
  float angleInDegrees = atan2(deltaY, deltaX) * 180 / 3.141;
  return angleInDegrees * -1;
}

void RaylibWrapper::render(
    const std::vector<std::vector<cv::Point2d>> &contours) {
  BeginDrawing();
  ClearBackground(RAYWHITE);

  BeginMode3D(camera);
  DrawGrid(10, 2.0f);

  int color_idx = 0;
  for (const auto &points : contours) {
    for (size_t i = 0; i < points.size(); i++) {
      float x1 = points[i].x, y1 = points[i].y;
      float x2 = points[(i + 1) % points.size()].x,
            y2 = points[(i + 1) % points.size()].y;

      float segment_distance = distance(x1, y1, x2, y2);
      Vector3 v1 = {x1, 0.0f, y1}, v2 = {x2, 0.0f, y2};
      float angle = angle_between_points(x1, y1, x2, y2);

      rlPushMatrix();
      rlTranslatef(v1.x, v1.y, v1.z);
      rlRotatef(angle, 0, 1, 0);
      DrawCube({segment_distance / 2, 0.0f, 0.0f}, segment_distance, 6.0f, 0.1f,
               colors[0]);
      rlPopMatrix();
    }
    color_idx++;
  }

  EndMode3D();
  EndDrawing();
}
