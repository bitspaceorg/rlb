#include "io.hpp"
#include "raylib.h"
#include "raylibwrapper.hpp"
#include "raymath.h"
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

int main() {
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  std::string image_path = "../test.jpeg";
  IOHelper *io = new IOHelper();
  auto img = io->read_image(image_path);
  std::vector<std::vector<cv::Point>> contours;
  std::vector<std::vector<cv::Point2d>> contours2d;
  img.water_shed(contours);
  img.normalize(contours, contours2d);

  RaylibWrapper viewer(800, 600, "3D Room Viewer");
  viewer.init();

  rlEnableDepthTest();
  rlEnableDepthMask();
  rlSetBlendMode(BLEND_ALPHA);

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

  Shader shader = LoadShader(TextFormat("../shader/lighting.vs", GLSL_VERSION),
                             TextFormat("../shader/lighting.fs", GLSL_VERSION));
  shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
  int ambientLoc = GetShaderLocation(shader, "ambient");
  SetShaderValue(shader, ambientLoc, (float[4]){1.0f, 1.0f, 1.0f, 1.0f},
                 SHADER_UNIFORM_VEC4);
  Light light[2] = {0};
  light[0] = CreateLight(LIGHT_POINT, (Vector3){-1, 3, -2}, Vector3Zero(), RED,
                         shader);
  light[1] = CreateLight(LIGHT_POINT, (Vector3){-1, 3, 45}, Vector3Zero(),
                         WHITE, shader);

  /*light[1] = CreateLight(LIGHT_POINT, (Vector3){100, 3, 45},
   * (Vector3){0,0,10},*/
  /*                       WHITE, shader);*/
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

    for (int i = 0; i < MAX_LIGHTS; i++) {
      if (light[i].enabled)
        DrawSphereEx(light[i].position, 0.2f, 8, 8, light[i].color);
      else
        DrawSphereWires(light[i].position, 0.2f, 8, 8,
                        ColorAlpha(light[i].color, 0.3f));
    }

    EndShaderMode();
    EndMode3D();
    EndDrawing();
  }
  UnloadShader(shader);
  return 0;
}
