// global shader declaration
#define GLSL_VERSION 330

#include "image.hpp"
#include "io.hpp"
#include "iostream"
#include "lights.hpp"
#include "raylib.h"
#include "raylibwrapper.hpp"
#include "raymath.h"
#include "rlImGui.h"
#include "toolbar.hpp"
#include "vignette.hpp"

// constants
const float floor_height = 6.0f;
int width = 1920, height = 1080;
float sumX = 0, sumY = 0;
Vector2 center = Vector2{width / 2.0f, height / 2.0f};

// globals
std::vector<std::vector<std::vector<cv::Point2d>>> floors;
std::vector<std::string> image_path = {};

void recalculate(const std::string &temp, RaylibWrapper &viewer) {
  floors.clear();
  sumX = 0;
  sumY = 0;
  IOHelper *io = new IOHelper();
  std::vector<std::vector<cv::Point>> contours;
  std::vector<std::vector<cv::Point2d>> contours2d;
  io->read_image(temp, contours);
  CustImage::normalize(contours, contours2d);
  floors.push_back(contours2d);
  // rendering
  std::vector<cv::Point2d> input_2D;
  for (const auto &points : floors[0]) {
    for (const auto &point : points) {
      input_2D.push_back(point);
    }
  }

  std::vector<cv::Point2d> boundary_ip = viewer.get_bounding_box(input_2D);
  for (const auto &point : boundary_ip) {
    sumX += point.x;
    sumY += point.y;
  }
  center = Vector2{sumX / 4.0f, sumY / 4.0f};
  viewer.cameras.clear();
  viewer.initialize_default_cam(center);
  viewer.initialize_floor_cam(floor_height, floors.size());
}

void enter() {
  // flags
  SetConfigFlags(FLAG_MSAA_4X_HINT);

  RaylibWrapper viewer(width, height, "3D Room Viewer");
  viewer.init();
  Toolbar::init(&viewer);

  viewer.initialize_default_cam(center);
  viewer.initialize_floor_cam(floor_height, floors.size());

  recalculate("../test_big.jpeg", viewer);
  Vignette vignette(width, height);

  // lighting
  LightLoader light = LightLoader(2, GLSL_VERSION);
  light.ShaderInit();
  light.InitializeLights(0, (Vector3){0, 0, 0}, Vector3Zero(), WHITE);
  light.InitializeLights(1, (Vector3){-1, 3, 45}, Vector3Zero(), WHITE);

  rlImGuiSetup(true);

  BoundingBox object = {(Vector3){-1.0f, 0.0f, -1.0f},
                        (Vector3){1.0f, 2.0f, 1.0f}};

  while (!WindowShouldClose()) {
    float cameraPos[3] = {viewer.get_camera().position.x,
                          viewer.get_camera().position.y,
                          viewer.get_camera().position.z};

    RaylibWrapper::listen(viewer, floors.size());

    BeginDrawing();
    ClearBackground(BLACK);

    viewer.update_camera();
    light.UpdateLight(viewer.get_camera());

    BeginMode3D(viewer.get_camera());
    light.EnableShader();
    RaylibWrapper::DrawFloor(viewer, floors);
    EndShaderMode();
    RaylibWrapper::DrawCeil(viewer, floors, floor_height);
    EndMode3D();

    vignette.EnableShader();
    vignette.Draw(!viewer.cameras[viewer.camera_index].is_toggle_sniper);
    vignette.DisableShader();

    rlImGuiBegin();
    Toolbar::render();
    rlImGuiEnd();

    EndDrawing();
  }

  light.DisableShader();
  rlImGuiShutdown();
}

int main() {
  enter();
  return 0;
}
