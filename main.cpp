#include "io.hpp"
#include "raylib.h"
#include "raylibwrapper.hpp"
#include "raymath.h"
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

int main() {
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  std::string image_path = "/Users/suryaprakash/Developer/sih/rlb/test.jpeg";
  IOHelper *io = new IOHelper();
  auto img = io->read_image(image_path);
  std::vector<std::vector<cv::Point>> contours;
  std::vector<std::vector<cv::Point2d>> contours2d;
  img.water_shed(contours);

  // TODO: fix this
  for (const auto &contour : contours) {
    std::vector<cv::Point2d> contour2d;
    std::cout << contour << "\n";
    for (const auto &pt : contour) {
      contour2d.emplace_back(pt.x, pt.y);
    }
    contours2d.push_back(contour2d);
  }

  RaylibWrapper viewer(800, 600, "3D Room Viewer");
  viewer.init();
  std::vector<cv::Point2d> input_2D;
  for (const auto &points : contours) {
    for (const auto &point : points) {
      input_2D.push_back(point);
    }
  }

  std::vector<cv::Point2d> boundary = viewer.get_bounding_box(input_2D);

  // #TODO: make relative path
  Shader shader = LoadShader(
      TextFormat("/Users/suryaprakash/Developer/sih/rlb/shader/lighting.vs",
                 GLSL_VERSION),
      TextFormat("/Users/suryaprakash/Developer/sih/rlb/shader/lighting.fs",
                 GLSL_VERSION));
  shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
  int ambientLoc = GetShaderLocation(shader, "ambient");
  SetShaderValue(shader, ambientLoc, (float[4]){1.0f, 1.0f, 1.0f, 1.0f},
                 SHADER_UNIFORM_VEC4);
  Light light = CreateLight(LIGHT_POINT, (Vector3){-1, 3, -2}, Vector3Zero(),
                            WHITE, shader);

  while (!WindowShouldClose()) {
    viewer.update_camera();
    float cameraPos[3] = {viewer.camera.position.x, viewer.camera.position.y,
                          viewer.camera.position.z};
    SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos,
                   SHADER_UNIFORM_VEC3);

    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode3D(viewer.get_camera());
    BeginShaderMode(shader);
    viewer.render(contours2d, viewer.colors[5]);
    viewer.render_base(boundary, 0.0f, viewer.colors[5]);
    EndShaderMode();
    EndMode3D();
    EndDrawing();
  }
  UnloadShader(shader);
  CloseWindow();
  delete io;
  return 0;
}
