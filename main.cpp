// global shader declaration
#define GLSL_VERSION 330

#include "io.hpp"
#include "raylib.h"
#include "raylibwrapper.hpp"
#include "vignette.hpp"
#include "lights.hpp"
#include "raymath.h"

int main() {
  // flags
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  const float floor_height = 6.0f;

  std::vector<std::vector<std::vector<cv::Point2d>>> floors;

  std::vector<std::string> image_path = {"../test.jpeg", "../test5.jpeg"};
  for (std::string i : image_path) {
    IOHelper *io = new IOHelper();
    auto img = io->read_image(i);
    std::vector<std::vector<cv::Point>> contours;
    std::vector<std::vector<cv::Point2d>> contours2d;
    img.water_shed(contours);
    CustImage::normalize(contours, contours2d);
    floors.push_back(contours2d);
  }

  int width = 1920, height = 1080;

  RaylibWrapper viewer(width, height, "3D Room Viewer");
  viewer.init();

  // rendering
  std::vector<cv::Point2d> input_2D;
  for (const auto &points : floors[0]) {
    for (const auto &point : points) {
      input_2D.push_back(point);
    }
  }
  std::vector<cv::Point2d> boundary_ip = viewer.get_bounding_box(input_2D);

  // camera.position = (Vector3){10.0f, 10.0f, 10.0f};
  // camera.target = (Vector3){2.5f, 0.0f, 2.5f};
  //
  float sumX = 0, sumY = 0;
  for (const auto &point : boundary_ip) {
    std::cout << point.x << " " << point.y << "\n";
    sumX += point.x;
    sumY += point.y;
  }

  Vector2 center = Vector2{sumX / 4.0f, sumY / 4.0f};
  Vignette vignette(width, height);

  // Free Flow
  viewer.add_camera(Vector3{10.0f, 10.0, 10.f}, Vector3{2.5f, 0.0f, 2.5f});
  // First Person
  viewer.add_camera(Vector3{-1.0f, 0.0f, -2.0f}, Vector3{2.5f, 0.0f, 2.5f},
                    45.0f, CAMERA_PERSPECTIVE, CAMERA_FIRST_PERSON);
  // Top Down
  // NOTE: to be changed - only for demo purposes
  viewer.add_camera(Vector3{center.x, 80.0f, center.y},
                    Vector3{center.x, -400.0f, center.x}, 45.0f,
                    CAMERA_PERSPECTIVE, CAMERA_CUSTOM);

  // lighting
  LightLoader *light = new LightLoader(2, GLSL_VERSION);
  light->ShaderInit();
  light->InitializeLights(0, (Vector3){0, 0, 0}, Vector3Zero(), WHITE);
  light->InitializeLights(1, (Vector3){-1, 3, 45}, Vector3Zero(), WHITE);

  while (!WindowShouldClose()) {
    float cameraPos[3] = {viewer.get_camera().position.x,
                          viewer.get_camera().position.y,
                          viewer.get_camera().position.z};

    if (IsKeyPressed(KEY_C))
      viewer.camera_index = ((viewer.camera_index + 1) % viewer.cameras.size());
    if (IsKeyPressed(KEY_TAB))
      viewer.cameras[viewer.camera_index].toggle_sniper_cam();

    BeginDrawing();
    ClearBackground(BLACK);

    viewer.update_camera();
    light->UpdateLight(viewer.get_camera());

    BeginMode3D(viewer.get_camera());
    light->EnableShader();

    float offset = 0.0f;

    for (const auto &contours2d : floors) {

      std::vector<cv::Point2d> input_2D;
      for (const auto &points : contours2d) {
        for (const auto &point : points) {
          input_2D.push_back(point);
        }
      }

      std::vector<cv::Point2d> boundary_ip = viewer.get_bounding_box(input_2D);

      Vector2dVector boundary;
      for (Point2d i : boundary_ip)
        boundary.push_back(Vector2d(i.x, i.y));

      viewer.render_base(boundary, offset + 0.1, viewer.colors[0]);
      viewer.render(contours2d, offset, 6.0f, viewer.colors[0]);
    }

    EndShaderMode();

    offset = 0.0f;

    for (const auto &floor : floors) {
      offset += floor_height;
      for (auto &contour : floor) {
        Vector2dVector points_ip;
        for (cv::Point2d point : contour) {
          points_ip.push_back(Vector2d(point.x, point.y));
        }
        viewer.render_base(points_ip, offset, viewer.colors[0]);
      }
    }

    EndMode3D();

    vignette.EnableShader();
    vignette.Draw(!viewer.cameras[viewer.camera_index].is_toggle_sniper);
    vignette.DisableShader();

    EndDrawing();
  }
  light->DisableShader();

  return 0;
}
