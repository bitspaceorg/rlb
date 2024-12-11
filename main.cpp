// global shader declaration
#include "imgui.h"
#define GLSL_VERSION 330

#include "io.hpp"
#include "lights.hpp"
#include "raylib.h"
#include "raylibwrapper.hpp"
#include "raymath.h"
#include "rlImGui.h"
#include "tinyfiledialogs.h"
#include "toolbar.hpp"
#include "vignette.hpp"

const float floor_height = 6.0f;
float sumX = 0, sumY = 0;
std::vector<std::vector<std::vector<cv::Point2d>>> floors;

void render_gloab_context(RaylibWrapper &viewer, LightLoader &light,
                          Vignette &vignette) {
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
}

void recalculate(std::vector<std::string> &image_path, RaylibWrapper &viewer) {
  floors.clear();
  for (std::string i : image_path) {
    IOHelper *io = new IOHelper();
    auto img = io->read_image(i);
    std::vector<std::vector<cv::Point>> contours;
    std::vector<std::vector<cv::Point2d>> contours2d;
    img.water_shed(contours);
    CustImage::normalize(contours, contours2d);
    floors.push_back(contours2d);
  }
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
}

int main() {
  // flags
  SetConfigFlags(FLAG_MSAA_4X_HINT);

  std::vector<std::string> image_path = {};

  int width = 1920, height = 1080;
  RaylibWrapper viewer(width, height, "3D Room Viewer");
  auto const addImage = [&]() {
    try {
      auto filePath =
          tinyfd_openFileDialog("Pick Image Path", ".", 0, NULL, NULL, 0);
      if (!filePath || filePath == NULL) {
      }
      image_path.push_back(filePath);
      recalculate(image_path, viewer);
    } catch (...) {
      // dont do anything here
    }
  };
  viewer.init();
  Toolbar::init(&viewer);
  Toolbar toolbar(addImage);
  Vector2 center = Vector2{sumX / 4.0f, sumY / 4.0f};

  viewer.initialize_default_cam(center);
  viewer.initialize_floor_cam(floor_height, floors.size());
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
    render_gloab_context(viewer, light, vignette);
    rlImGuiBegin();
    toolbar.render();
    rlImGuiEnd();
    EndDrawing();
  }
  light.DisableShader();
  rlImGuiShutdown();

  return 0;
}
