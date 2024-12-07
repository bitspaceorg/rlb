// global shader declaration
#define GLSL_VERSION 330

#include "io.hpp"
#include "raylib.h"
#include "raylibwrapper.hpp"
/*#define RLIGHTS_IMPLEMENTATION*/
#include "lights.hpp"
#include "raymath.h"

int main() {
  // flags
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  const float floor_height = 6.0f;

  std::vector<std::vector<std::vector<cv::Point2d>>> floors;

  std::vector<std::string> image_path = {"../test.jpeg"};
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
  light->InitializeLights(0, (Vector3){-1, 3, -2}, Vector3Zero(), WHITE);
  light->InitializeLights(1, (Vector3){-1, 3, 45}, Vector3Zero(), WHITE);

  Shader vignette_shader = LoadShader(0, TextFormat("../shader/vignette.fs"));

  int rLoc = GetShaderLocation(vignette_shader, "radius");
  int blurLoc = GetShaderLocation(vignette_shader, "blur");

  int colLoc = GetShaderLocation(vignette_shader, "color");

  float radius = 0.3f;
  float blur = 0.0f;
  Vector3 vColor = {0.0f, 0.0f, 0.0f};

  RenderTexture2D vTexture = LoadRenderTexture(width, height);

  while (!WindowShouldClose()) {
    float cameraPos[3] = {viewer.get_camera().position.x,
                          viewer.get_camera().position.y,
                          viewer.get_camera().position.z};
    SetShaderValue(vignette_shader, rLoc, &radius, SHADER_UNIFORM_FLOAT);
    SetShaderValue(vignette_shader, blurLoc, &blur, SHADER_UNIFORM_FLOAT);
    SetShaderValue(vignette_shader, colLoc, &vColor, SHADER_UNIFORM_VEC3);

    if (IsKeyPressed(KEY_C))
      viewer.camera_index = ((viewer.camera_index + 1) % viewer.cameras.size());
    if (IsKeyPressed(KEY_TAB))
      viewer.cameras[viewer.camera_index].toggle_sniper_cam();

    BeginDrawing();
    ClearBackground(BLACK);

    viewer.update_camera();

    BeginMode3D(viewer.get_camera());

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

      viewer.render_base(boundary, offset, viewer.colors[0]);
      viewer.render(contours2d, offset, 6.0f, viewer.colors[0]);
    }

    // for (int i = 0; i < MAX_LIGHTS; i++) {
    //   if (light[i].enabled)
    //     DrawSphereEx(light[i].position, 0.2f, 8, 8, light[i].color);
    //   else
    //     DrawSphereWires(light[i].position, 0.2f, 8, 8,
    //                     ColorAlpha(light[i].color, 0.3f));
    // }

    EndShaderMode();

    for (const auto &floor : floors) {
      for (auto &contour : floor) {
        Vector2dVector points_ip;
        for (cv::Point2d point : contour) {
          points_ip.push_back(Vector2d(point.x, point.y));
        }
        for (auto &point : contour) {
          viewer.render_base(points_ip, 6.0, viewer.colors[0]);
        }
      }
    }

    EndMode3D();

    BeginShaderMode(vignette_shader);
    if (!viewer.cameras[viewer.camera_index].is_toggle_sniper) {
      DrawTextureRec(vTexture.texture,
                     (Rectangle){0, 0, vTexture.texture.width * 1.0f,
                                 -vTexture.texture.height * 1.0f},
                     (Vector2){0, 0}, BLANK);
      Vector2 centerScreen = {width / 2.0f, height / 2.0f};
      DrawCircleV(centerScreen, 2.0f, RED);
    }
    EndShaderMode();

    EndDrawing();
  }
  // Destructors
  return 0;
}
