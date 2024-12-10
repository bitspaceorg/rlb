#include "raylibwrapper.hpp"
#include "camera.hpp"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "triangulate.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

RaylibWrapper::RaylibWrapper(int width, int height, const std::string &title)
    : window_width(width), window_height(height), window_title(title) {}

RaylibWrapper::~RaylibWrapper() { CloseWindow(); }

void RaylibWrapper::init() {
  InitWindow(window_width, window_height, window_title.c_str());
  SetTargetFPS(60);
}

void RaylibWrapper::update_camera() {
  Camera3D &camera = get_camera();
  int mode = get_camera_mode();

  cameras[camera_index].prev_pos = camera.position;

  DrawSphere(camera.position, 1.0f, RED);

  UpdateCamera(&camera, mode);
}

void RaylibWrapper::add_camera(const Vector3 &position, const Vector3 &target,
                               float initialFov, int projection, int mode,
                               bool is_target_camera) {
  cameras.push_back(CameraRay(position, target, initialFov, projection, mode,
                              is_target_camera));
}

void RaylibWrapper::initialize_default_cam(const Vector2 &center) {
  /* Free Flow */
  add_camera(Vector3{10.0f, 10.0, 10.f}, Vector3{2.5f, 0.0f, 2.5f});

  /*
   * Top Down
   * NOTE: to be changed - only for demo purposes
   */
  add_camera(Vector3{center.x, 80.0f, center.y},
             Vector3{center.x, -400.0f, center.x}, 45.0f, CAMERA_PERSPECTIVE,
             CAMERA_CUSTOM);
}

void RaylibWrapper::initialize_floor_cam(const float &height,
                                         const int &floor_count) {
  float offset = 0.0f;
  for (int i = 0; i < floor_count; i++) {
    /*
     * First Person
     * NOTE: customize the initial points of the floor
     */
    add_camera(Vector3{-1.0f, offset, -4.0f}, Vector3{2.5f, offset, 2.5f},
               45.0f, CAMERA_PERSPECTIVE, CAMERA_FIRST_PERSON);
    offset += height;
  }
}

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

bool RaylibWrapper::intersectRayBox(const Ray &ray, const Vector3 &boxMin,
                                    const Vector3 &boxMax, float &tMin,
                                    float &tMax) {
  tMin = 0.0f;
  tMax = FLT_MAX;

  for (int i = 0; i < 3; i++) {
    float t1, t2;

    if (i == 0) {
      t1 = (boxMin.x - ray.position.x) / ray.direction.x;
      t2 = (boxMax.x - ray.position.x) / ray.direction.x;
    } else if (i == 1) {
      t1 = (boxMin.y - ray.position.y) / ray.direction.y;
      t2 = (boxMax.y - ray.position.y) / ray.direction.y;
    } else {
      t1 = (boxMin.z - ray.position.z) / ray.direction.z;
      t2 = (boxMax.z - ray.position.z) / ray.direction.z;
    }

    if (t1 > t2)
      std::swap(t1, t2);

    tMin = std::max(tMin, t1);
    tMax = std::min(tMax, t2);

    if (tMin > tMax)
      return false;
  }
  return true;
}

// void DrawBoundingBox(const BoundingBox &box, Color color) {
//   Vector3 corners[8] = {
//       {box.min.x, box.min.y, box.min.z}, {box.max.x, box.min.y, box.min.z},
//       {box.max.x, box.max.y, box.min.z}, {box.min.x, box.max.y, box.min.z},
//       {box.min.x, box.min.y, box.max.z}, {box.max.x, box.min.y, box.max.z},
//       {box.max.x, box.max.y, box.max.z}, {box.min.x, box.max.y, box.max.z}};
//
//   DrawLine3D(corners[0], corners[1], color);
//   DrawLine3D(corners[1], corners[2], color);
//   DrawLine3D(corners[2], corners[3], color);
//   DrawLine3D(corners[3], corners[0], color);
//
//   DrawLine3D(corners[4], corners[5], color);
//   DrawLine3D(corners[5], corners[6], color);
//   DrawLine3D(corners[6], corners[7], color);
//   DrawLine3D(corners[7], corners[4], color);
//
//   DrawLine3D(corners[0], corners[4], color);
//   DrawLine3D(corners[1], corners[5], color);
//   DrawLine3D(corners[2], corners[6], color);
//   DrawLine3D(corners[3], corners[7], color);
// }
//
//
BoundingBox RaylibWrapper::GetRotatedCubeBoundingBox(float x1, float y1,
                                                     float x2, float y2,
                                                     float height,
                                                     float offset) {
  float segment_distance = distance(x1, y1, x2, y2);
  float angle = angle_between_points(x1, y1, x2, y2) * DEG2RAD;

  Vector3 cubeCorners[8] = {
      {0, 0, -0.05f},      {segment_distance, 0, -0.05f},
      {0, 0, 0.05f},       {segment_distance, 0, 0.05f},
      {0, height, -0.05f}, {segment_distance, height, -0.05f},
      {0, height, 0.05f},  {segment_distance, height, 0.05f}};

  Matrix rotation = MatrixRotateY(angle);
  Vector3 translation = {x1, offset, y1};

  BoundingBox box = {{INFINITY, INFINITY, INFINITY},
                     {-INFINITY, -INFINITY, -INFINITY}};

  for (int i = 0; i < 8; i++) {
    Vector3 transformed = Vector3Transform(cubeCorners[i], rotation);
    transformed = Vector3Add(transformed, translation);
    box.min = Vector3Min(box.min, transformed);
    box.max = Vector3Max(box.max, transformed);
  }

  return box;
}
void RaylibWrapper::render(
    const std::vector<std::vector<cv::Point2d>> &contours, float &offset,
    const float &height, Color color) {

  for (const auto &points : contours) {
    for (size_t i = 0; i < points.size(); i++) {

      float x1 = points[i].x, y1 = points[i].y;
      float x2 = points[(i + 1) % points.size()].x,
            y2 = points[(i + 1) % points.size()].y;

      float segment_distance = distance(x1, y1, x2, y2);
      Vector3 v1 = {x1, offset, y1}, v2 = {x2, offset, y2};
      float angle = angle_between_points(x1, y1, x2, y2);

      float cubeHeight = height;
      float cubeWidth = segment_distance;

      rlPushMatrix();
      rlTranslatef(v1.x, v1.y, v1.z);
      rlRotatef(angle, 0, 1, 0);
      DrawCube({cubeWidth / 2, 0.0f, 0.0f}, cubeWidth, cubeHeight, 0.1f, color);
      rlPopMatrix();

      BoundingBox cube_box =
          GetRotatedCubeBoundingBox(x1, y1, x2, y2, cubeHeight, offset - 3.0f);

      // DrawBoundingBox(cube_box, RED);

      if (CheckCollisionBoxSphere(cube_box, get_camera().position, 0.1f)) {
        std::cout << "Collision detected!\n";
        get_camera().position = cameras[camera_index].prev_pos;
      }
    }
  }

  offset += height;
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

Vector2dVector preprocessContours(const Vector2dVector &contours) {
  Vector2dVector cleanContours;
  for (size_t i = 0; i < contours.size(); ++i) {
    if (i == 0 || !(contours[i].GetX() == contours[i - 1].GetX() &&
                    contours[i].GetY() == contours[i - 1].GetY())) {
      cleanContours.push_back(contours[i]);
    }
  }
  return cleanContours;
}

void RaylibWrapper::render_base(const Vector2dVector &contours, float y,
                                Color color) {
  Vector2dVector preprocess_contours = preprocessContours(contours);
  Vector2dVector result;
  Triangulate::Process(preprocess_contours, result);
  int tcount = result.size() / 3;

  y -= 3.0f;

  for (int i = 0; i < tcount; i++) {
    Vector2d p1 = result[i * 3 + 0];
    Vector2d p2 = result[i * 3 + 1];
    Vector2d p3 = result[i * 3 + 2];
    if (p1.GetX() == p2.GetX() && p1.GetY() == p2.GetY() ||
        p2.GetX() == p3.GetX() && p2.GetY() == p3.GetY() ||
        p3.GetX() == p1.GetX() && p3.GetY() == p1.GetY())
      continue;
    DrawTriangle3D(Vector3{p1.GetX(), y, p1.GetY()},
                   Vector3{p3.GetX(), y, p3.GetY()},
                   Vector3{p2.GetX(), y, p2.GetY()}, color);
    DrawTriangle3D(Vector3{p1.GetX(), y, p1.GetY()},
                   Vector3{p2.GetX(), y, p2.GetY()},
                   Vector3{p3.GetX(), y, p3.GetY()}, color);
    // printf("Triangle %d => (%0.0f,%0.0f) (%0.0f,%0.0f) (%0.0f,%0.0f)\n", i +
    // 1,
    //        p1.GetX(), p1.GetY(), p2.GetX(), p2.GetY(), p3.GetX(), p3.GetY());
  }
  // printf("\n");
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
        boundary.pop_back();
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

void RaylibWrapper::listen(RaylibWrapper &viewer, const int &floor_count) {
  const int default_cam = 2;
  if (IsKeyPressed(KEY_C))
    viewer.camera_index = ((viewer.camera_index + 1) % default_cam);
  if (IsKeyUp(KEY_TAB)) {
    viewer.cameras[viewer.camera_index].toggle_sniper_cam(true);
  }

  for (int i = 0; i < floor_count; i++) {
#if defined(__APPLE__)
    bool modifier_held =
        IsKeyDown(KEY_LEFT_SUPER) || IsKeyDown(KEY_RIGHT_SUPER);
#else
    bool modifier_held =
        IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
#endif

    if (IsKeyPressed(i + 49) && modifier_held) {
      viewer.camera_index = i + default_cam;
    }
  }

  for (int i = floor_count + default_cam; i < viewer.cameras.size(); i++) {
#if defined(__APPLE__)
    bool modifier_held =
        IsKeyDown(KEY_LEFT_SUPER) || IsKeyDown(KEY_RIGHT_SUPER);
#else
    bool modifier_held =
        IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
#endif
    if (IsKeyPressed(i + 49 - default_cam) && modifier_held) {
      viewer.camera_index = i;
    }
  }

  if (IsKeyDown(KEY_TAB)) {
    viewer.cameras[viewer.camera_index].toggle_sniper_cam(false);
  }
}

Vector3 RaylibWrapper::GetRayHitPosition(Ray ray, float distance) {
  float length = sqrtf(ray.direction.x * ray.direction.x +
                       ray.direction.y * ray.direction.y +
                       ray.direction.z * ray.direction.z);

  Vector3 normalizedDir = {ray.direction.x / length, ray.direction.y / length,
                           ray.direction.z / length};

  Vector3 scaledDir = {normalizedDir.x * distance, normalizedDir.y * distance,
                       normalizedDir.z * distance};

  Vector3 hitPosition = {ray.position.x + scaledDir.x,
                         ray.position.y + scaledDir.y,
                         ray.position.z + scaledDir.z};

  return hitPosition;
}

void RaylibWrapper::DrawFloor(
    RaylibWrapper &viewer,
    std::vector<std::vector<std::vector<cv::Point2d>>> &floors) {
  float offset = 0.0f;

  for (const auto &contours2d : floors) {

    std::vector<cv::Point2d> input_2D;

    int index = 0;

    for (const auto &points : contours2d) {

      Ray ray = GetMouseRay(
          (Vector2){viewer.window_width / 2.0f, viewer.window_height / 2.0f},
          viewer.get_camera());

      for (const auto &point : points) {
        input_2D.push_back(point);
      }
    }

    std::vector<cv::Point2d> boundary_ip = viewer.get_bounding_box(input_2D);

    Vector2dVector boundary;
    for (cv::Point2d i : boundary_ip)
      boundary.push_back(Vector2d(i.x, i.y));

    viewer.render_base(boundary, offset + 0.1, viewer.colors[0]);
    viewer.render(contours2d, offset, 6.0f, viewer.colors[0]);
  }
}

void RaylibWrapper::DrawCeil(
    RaylibWrapper &viewer,
    std::vector<std::vector<std::vector<cv::Point2d>>> &floors,
    const float &floor_height) {

  float offset = 0.0f;

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
}
