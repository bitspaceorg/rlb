#pragma once

#include <opencv2/core.hpp>
#include <vector>
#include "raylib.h"
#include "rlgl.h"


#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif


class RaylibWrapper {
private:
  int camera_mode;
  int window_width;
  int window_height;
  std::string window_title;

  float distance(float x1, float y1, float x2, float y2);
  float angle_between_points(float p1x, float p1y, float p2x, float p2y);
  float cross(const cv::Point2d &O, const cv::Point2d &A, const cv::Point2d &B);

public:
  Camera3D camera;
  RaylibWrapper(int width, int height, const std::string &title);
  ~RaylibWrapper();

  void init();
  void update_camera();
  Color colors[6] = {DARKGRAY, PINK, ORANGE, DARKBLUE, GREEN, GRAY};
  Camera3D get_camera() { return camera; };
  void render(const std::vector<std::vector<cv::Point2d>> &contours, Color color);
  void render_base(const std::vector<cv::Point2d> &contours, float z, Color color);
  void render_base_lines(const std::vector<cv::Point2d> &contours, float z, Color color);
  std::vector<cv::Point2d> get_convex_hull(std::vector<cv::Point2d> &points);
  std::vector<cv::Point2d> get_boundary(std::vector<cv::Point2d> &points);
  std::vector<cv::Point2d> get_bounding_box(std::vector<cv::Point2d> &points);
  std::vector<cv::Point2d> get_closed_polygon(std::vector<cv::Point2d> &points);
};
