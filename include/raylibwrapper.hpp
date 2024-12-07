#pragma once

#include "camera.hpp"
#include "raylib.h"
#include "rlgl.h"
#include "triangulate.hpp"
#include <opencv2/core.hpp>
#include <vector>


class RaylibWrapper {
private:
  int window_width;
  int window_height;
  std::string window_title;

  float distance(float x1, float y1, float x2, float y2);
  Vector2 GetPolygonCenter(const std::vector<Vector2> &points);
  float angle_between_points(float p1x, float p1y, float p2x, float p2y);
  float cross(const cv::Point2d &O, const cv::Point2d &A, const cv::Point2d &B);

public:
  RaylibWrapper(int width, int height, const std::string &title);
  ~RaylibWrapper();

  std::vector<CameraRay> cameras;
  int camera_index = 0;

  void init();

  static void listen(RaylibWrapper &viewer);
  static void DrawFloor(RaylibWrapper &viewer, std::vector<std::vector<std::vector<cv::Point2d>>> &floors);
  static void DrawCeil(RaylibWrapper &viewer, std::vector<std::vector<std::vector<cv::Point2d>>> &floors);

  void update_camera();
  Camera3D &get_camera() { return cameras[camera_index].get_camera(); }
  int get_camera_mode() { return cameras[camera_index].get_camera_mode(); }
  void add_camera(const Vector3 &position, const Vector3 &target,
                  float initialFov = 45.0f, int projection = CAMERA_PERSPECTIVE,
                  int mode = CAMERA_FREE);
  Color colors[6] = {DARKGRAY, PINK, ORANGE, DARKBLUE, GREEN, GRAY};
  void render(const std::vector<std::vector<cv::Point2d>> &contours,
              float &offset, const float &height, Color color);
  void render_base(const Vector2dVector &polygon, float y, Color color);
  void render_base_lines(const std::vector<cv::Point2d> &contours, float z,
                         Color color);
  std::vector<cv::Point2d> get_convex_hull(std::vector<cv::Point2d> &points);
  std::vector<cv::Point2d> get_boundary(std::vector<cv::Point2d> &points);
  std::vector<cv::Point2d> get_bounding_box(std::vector<cv::Point2d> &points);
  std::vector<cv::Point2d> get_closed_polygon(std::vector<cv::Point2d> &points);
};
