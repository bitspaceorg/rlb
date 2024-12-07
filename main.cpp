//global shader declaration
#define GLSL_VERSION 330


#include "io.hpp"
#include "raylib.h"
#include "raylibwrapper.hpp"
/*#define RLIGHTS_IMPLEMENTATION*/
#include "lights.hpp"
#include "raymath.h"


int main() {
	//flags
  SetConfigFlags(FLAG_MSAA_4X_HINT);


	//image processing
  std::string image_path = "../test.jpeg";
  IOHelper *io = new IOHelper();
  auto img = io->read_image(image_path);
  std::vector<std::vector<cv::Point>> contours;
  std::vector<std::vector<cv::Point2d>> contours2d;
  img.water_shed(contours);
  CustImage::normalize(contours, contours2d);


  RaylibWrapper viewer(800, 600, "3D Room Viewer");
  viewer.init();

  rlEnableDepthTest();
  rlEnableDepthMask();
  rlSetBlendMode(BLEND_ALPHA);

	//rendering
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

	//lighting
	LightLoader *light = new LightLoader(2,GLSL_VERSION);
	light->ShaderInit();
	light->InitializeLights(0,(Vector3){-1,3,-2},Vector3Zero(),WHITE);
	light->InitializeLights(1,(Vector3){-1,3,45},Vector3Zero(),WHITE);


  while (!WindowShouldClose()) {
    viewer.update_camera();
    BeginDrawing();
    ClearBackground(BLACK);

		light->UpdateLight(viewer.camera);
    BeginMode3D(viewer.get_camera());
		light->EnableShader();


    viewer.render(contours2d, viewer.colors[5]);
    viewer.render_base(boundary, 0.0f, viewer.colors[5]);
    //debug light
    //light->EnableDebug();

    EndShaderMode();

    for (auto &contour: contours2d) {
        Vector2dVector points_ip;
        for (cv::Point2d point : contour) {
          points_ip.push_back(Vector2d(point.x, point.y));
        }
        for (auto &point: contours2d) {
            viewer.render_base(points_ip, 6.0, WHITE);
        }
    }

    EndMode3D();
    EndDrawing();
  }
	//Destructors
	light->DisableShader();
  return 0;
}
