#pragma once

#include <opencv2/core.hpp>
#include <vector>
#include "raylib.h"
#include "rlgl.h"

class RaylibWrapper {
private:
  Camera3D camera;
  int camera_mode;
  Color colors[5] = {RED, PINK, ORANGE, DARKBLUE, GREEN};
  int window_width;
  int window_height;
  std::string window_title;

  float distance(float x1, float y1, float x2, float y2);
  float angle_between_points(float p1x, float p1y, float p2x, float p2y);

public:
  RaylibWrapper(int width, int height, const std::string &title);
  ~RaylibWrapper();

  void init();
  void update_camera();
  void render(const std::vector<std::vector<cv::Point2d>> &contours);
};
