#include "raylibwrapper.hpp"
#include "raylib.h"
#include "rlgl.h"
#include "triangulate.hpp"
#include <cmath>

RaylibWrapper::RaylibWrapper(int width, int height, const std::string &title)
    : window_width(width), window_height(height), window_title(title) {
  camera = {0};
  camera.position = (Vector3){10.0f, 10.0f, 10.0f};
  camera.target = (Vector3){2.5f, 0.0f, 2.5f};
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  camera.fovy = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;
  camera_mode = CAMERA_FREE;
}

RaylibWrapper::~RaylibWrapper() { CloseWindow(); }

void RaylibWrapper::init() {
  InitWindow(window_width, window_height, window_title.c_str());
  SetTargetFPS(60);
}

void RaylibWrapper::update_camera() { UpdateCamera(&camera, camera_mode); }

float RaylibWrapper::distance(float x1, float y1, float x2, float y2) {
  return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

float RaylibWrapper::cross(const cv::Point2d &O, const cv::Point2d &A,
                           const cv::Point2d &B) {
  return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

float RaylibWrapper::angle_between_points(float p1x, float p1y, float p2x,
                                          float p2y) {
  float deltaY = p2y - p1y;
  float deltaX = p2x - p1x;
  float angleInDegrees = atan2(deltaY, deltaX) * 180 / 3.141;
  return angleInDegrees * -1;
}

void RaylibWrapper::render(
    const std::vector<std::vector<cv::Point2d>> &contours, Color color) {

  for (const auto &points : contours) {
    for (size_t i = 0; i < points.size(); i++) {

      float x1 = points[i].x, y1 = points[i].y;
      float x2 = points[(i + 1) % points.size()].x,
            y2 = points[(i + 1) % points.size()].y;

      float segment_distance = distance(x1, y1, x2, y2);
      Vector3 v1 = {x1, 0.0f, y1}, v2 = {x2, 0.0f, y2};
      float angle = angle_between_points(x1, y1, x2, y2);

      rlPushMatrix();
      rlTranslatef(v1.x, v1.y, v1.z);
      rlRotatef(angle, 0, 1, 0);

      DrawCube({segment_distance / 2, 0.0f, 0.0f}, segment_distance, 6.0f, 0.1f,
               color);

      rlPopMatrix();
    }

    // NOTE: Enable if all points are accurate enough (solid ceiling)
    // Vector2dVector points_ip;
    // for (cv::Point2d point : points) {
    //   points_ip.push_back(Vector2d(point.x, point.y));
    // }
    // auto points_ip = points;
    // render_base(points_ip, 6.0f, WHITE);
    // render_base_lines(get_closed_polygon(points_ip), 6.0f, colors[0]);
  }
}

void RaylibWrapper::render_base_lines(const std::vector<cv::Point2d> &contours,
                                      float z, Color color) {
  for (int i = 1; i < contours.size() - 1; i++) {
    DrawLine3D(Vector3{static_cast<float>(contours[i].x), z,
                       static_cast<float>(contours[i].y)},
               Vector3{static_cast<float>(contours[i + 1].x), z,
                       static_cast<float>(contours[i + 1].y)},
               color);
  }
  DrawLine3D(Vector3{static_cast<float>(contours[contours.size() - 1].x), z,
                     static_cast<float>(contours[contours.size() - 1].y)},
             Vector3{static_cast<float>(contours[0].x), z,
                     static_cast<float>(contours[0].y)},
             color);
}

void RaylibWrapper::render_base(const Vector2dVector &contours, float y,
                                Color color) {
  Vector2dVector result;
  Triangulate::Process(contours, result);
  int tcount = result.size() / 3;

  y -= 3.0f;

  for (int i = 0; i < tcount; i++) {
    Vector2d p1 = result[i * 3 + 0];
    Vector2d p2 = result[i * 3 + 1];
    Vector2d p3 = result[i * 3 + 2];
    DrawTriangle3D(Vector3{p1.GetX(), y, p1.GetY()},
                   Vector3{p3.GetX(), y, p3.GetY()},
                   Vector3{p2.GetX(), y, p2.GetY()}, color);
    // printf("Triangle %d => (%0.0f,%0.0f) (%0.0f,%0.0f) (%0.0f,%0.0f)\n", i +
    // 1,
    //        p1.GetX(), p1.GetY(), p2.GetX(), p2.GetY(), p3.GetX(), p3.GetY());
  }
}

std::vector<cv::Point2d>
RaylibWrapper::get_convex_hull(std::vector<cv::Point2d> &points) {
  if (points.size() < 3)
    return points;

  std::sort(points.begin(), points.end(),
            [](const cv::Point2d &a, const cv::Point2d &b) {
              return (a.x < b.x) || (a.x == b.x && a.y < b.y);
            });

  std::vector<cv::Point2d> hull;

  for (const auto &pt : points) {
    while (hull.size() >= 2 &&
           cross(hull[hull.size() - 2], hull[hull.size() - 1], pt) <= 0) {
      hull.pop_back();
    }
    hull.push_back(pt);
  }

  size_t lower_size = hull.size();
  for (auto it = points.rbegin(); it != points.rend(); ++it) {
    while (hull.size() > lower_size &&
           cross(hull[hull.size() - 2], hull[hull.size() - 1], *it) <= 0) {
      hull.pop_back();
    }
    hull.push_back(*it);
  }

  hull.pop_back();

  return hull;
}

std::vector<cv::Point2d>
RaylibWrapper::get_boundary(std::vector<cv::Point2d> &points) {
  if (points.size() <= 3)
    return points;

  std::sort(points.begin(), points.end(),
            [](const cv::Point2d &a, const cv::Point2d &b) {
              return (a.x < b.x) || (a.x == b.x && a.y < b.y);
            });

  std::vector<cv::Point2d> boundary;

  for (const auto &p : points) {
    while (boundary.size() >= 2) {
      cv::Point2d p1 = boundary[boundary.size() - 2];
      cv::Point2d p2 = boundary.back();
      double cross =
          (p2.x - p1.x) * (p.y - p2.y) - (p2.y - p1.y) * (p.x - p2.x);
      if (cross <= 0)
        boundary.pop_back(); // Remove non-boundary point
      else
        break;
    }
    boundary.push_back(p);
  }

  size_t lower_size = boundary.size();
  for (auto it = points.rbegin(); it != points.rend(); ++it) {
    while (boundary.size() > lower_size) {
      cv::Point2d p1 = boundary[boundary.size() - 2];
      cv::Point2d p2 = boundary.back();
      double cross =
          (p2.x - p1.x) * (it->y - p2.y) - (p2.y - p1.y) * (it->x - p2.x);
      if (cross <= 0)
        boundary.pop_back(); // Remove non-boundary point
      else
        break;
    }
    boundary.push_back(*it);
  }

  boundary.pop_back();
  return boundary;
}

std::vector<cv::Point2d>
RaylibWrapper::get_bounding_box(std::vector<cv::Point2d> &points) {
  if (points.empty())
    return {};

  double minX = points[0].x, maxX = points[0].x;
  double minY = points[0].y, maxY = points[0].y;

  for (const auto &point : points) {
    if (point.x < minX)
      minX = point.x;
    if (point.x > maxX)
      maxX = point.x;
    if (point.y < minY)
      minY = point.y;
    if (point.y > maxY)
      maxY = point.y;
  }

  std::vector<cv::Point2d> boundingBox = {
      {minX, minY}, {maxX, minY}, {maxX, maxY}, {minX, maxY}};

  return boundingBox;
}

std::vector<cv::Point2d>
RaylibWrapper::get_closed_polygon(std::vector<cv::Point2d> &points) {
  if (points.size() < 3)
    return points;

  std::vector<cv::Point2d> polygon;

  for (const auto &p : points) {
    while (polygon.size() >= 2) {
      cv::Point2d p1 = polygon[polygon.size() - 2];
      cv::Point2d p2 = polygon.back();
      double cross =
          (p2.x - p1.x) * (p.y - p2.y) - (p2.y - p1.y) * (p.x - p2.x);
      if (cross <= 0)
        polygon.pop_back();
      else
        break;
    }
    polygon.push_back(p);
  }

  size_t lower_size = polygon.size();
  for (auto it = points.rbegin(); it != points.rend(); ++it) {
    while (polygon.size() > lower_size) {
      cv::Point2d p1 = polygon[polygon.size() - 2];
      cv::Point2d p2 = polygon.back();
      double cross =
          (p2.x - p1.x) * (it->y - p2.y) - (p2.y - p1.y) * (it->x - p2.x);
      if (cross <= 0)
        polygon.pop_back();
      else
        break;
    }
    polygon.push_back(*it);
  }

  polygon.pop_back();

  return polygon;
}
