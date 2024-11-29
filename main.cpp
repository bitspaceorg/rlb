#include "raylibwrapper.hpp"
#include "io.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#include <iostream>

int main(int, char **) {
  std::string image_path = "/Users/suryaprakash/Developer/sih/test.jpeg";
  IOHelper *io = new IOHelper();
  auto img = io->read_image(image_path);
  std::vector<std::vector<cv::Point2d>> contours;
  img.water_shed(contours);

  RaylibWrapper viewer(800, 600, "3D Room Viewer");
  viewer.init();

  while (!WindowShouldClose()) {
      viewer.update_camera();
      viewer.render(contours);
  }
};
