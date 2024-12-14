#pragma once

#include "raylib.h"
#include "raylibwrapper.hpp"
#include "raymath.h"
#include "rlgl.h"
#include <vector>

static RaylibWrapper *viewer;

struct Sphere {
  Vector3 position;
  float radius;
  bool selected;

  Sphere(Vector3 pos, float r) : position(pos), radius(r), selected(false) {}
};

struct Connection {
  int first;
  int second;
  float rotationAngle;

  Connection(int f, int s) : first(f), second(s), rotationAngle(0.0f) {}
};

class SphereSystem {
private:
  std::vector<Sphere> spheres;
  std::vector<Connection> connections;
  float sphereRadius;
  int selectedSphere;
  float axisLength;
  float moveSpeed;
  int stepsBetweenConnections;

public:
  SphereSystem(float radius = 1.0f, RaylibWrapper *view = nullptr,
               int steps = 10)
      : sphereRadius(radius), selectedSphere(-1), axisLength(2.0f),
        moveSpeed(0.1f), stepsBetweenConnections(steps) {
    viewer = view;
  }

  void Update() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        viewer->is_add_stair_points) {
      Ray ray = GetMouseRay(GetMousePosition(), viewer->get_camera());
      for (size_t i = 0; i < spheres.size(); i++) {
        RayCollision collision =
            GetRayCollisionSphere(ray, spheres[i].position, sphereRadius);

        if (collision.hit) {
          if (selectedSphere == i) {
            selectedSphere = -1;
            spheres[i].selected = false;
          } else {
            if (selectedSphere >= 0) {
              spheres[selectedSphere].selected = false;
            }
            selectedSphere = i;
            spheres[i].selected = true;
          }
          return;
        }
      }
      Vector3 planeNormal = {0.0f, 1.0f, 0.0f};
      Vector3 planePoint = {0.0f, 0.0f, 0.0f};
      RayCollision planeHit =
          GetRayCollision_Plane(ray, planePoint, planeNormal);

      if (planeHit.hit) {
        spheres.emplace_back(planeHit.point, sphereRadius);
      }
    }
    if (selectedSphere >= 0) {
      if (IsKeyDown(KEY_RIGHT))
        spheres[selectedSphere].position.x += moveSpeed;
      if (IsKeyDown(KEY_LEFT))
        spheres[selectedSphere].position.x -= moveSpeed;
      if (IsKeyDown(KEY_UP))
        spheres[selectedSphere].position.y += moveSpeed;
      if (IsKeyDown(KEY_DOWN))
        spheres[selectedSphere].position.y -= moveSpeed;
      if (IsKeyDown(KEY_M))
        spheres[selectedSphere].position.z += moveSpeed;
      if (IsKeyDown(KEY_N))
        spheres[selectedSphere].position.z -= moveSpeed;
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
      Ray ray = GetMouseRay(GetMousePosition(), viewer->get_camera());
      for (size_t i = 0; i < spheres.size(); i++) {
        RayCollision collision =
            GetRayCollisionSphere(ray, spheres[i].position, sphereRadius);

        if (collision.hit) {
          if (selectedSphere == -1) {
            selectedSphere = i;
            spheres[i].selected = true;
          } else if (selectedSphere != i) {
            connections.emplace_back(selectedSphere, i);
            spheres[selectedSphere].selected = false;
            selectedSphere = -1;
          }
          break;
        }
      }
    }

    if (IsKeyDown(KEY_B)) {
      for (auto &connection : connections) {
        connection.rotationAngle += 1.0f;
        if (connection.rotationAngle >= 360.0f) {
          connection.rotationAngle -= 360.0f;
        }
      }
    }
  }

  RayCollision GetRayCollision_Plane(Ray ray, Vector3 planePos,
                                     Vector3 planeNorm) {
    RayCollision collision = {0};

    float denom = Vector3DotProduct(planeNorm, ray.direction);
    if (fabs(denom) > 0.0001f) {
      Vector3 diff = Vector3Subtract(planePos, ray.position);
      float t = Vector3DotProduct(diff, planeNorm) / denom;
      if (t >= 0.0f) {
        collision.hit = true;
        collision.distance = t;
        collision.point =
            Vector3Add(ray.position, Vector3Scale(ray.direction, t));
        collision.normal = planeNorm;
      }
    }

    return collision;
  }

  void Draw() {
    if (viewer->is_add_stair_points) {
      for (const auto &sphere : spheres) {
        Color color = sphere.selected ? RED : BLUE;
        DrawSphere(sphere.position, sphereRadius, color);
        DrawSphereWires(sphere.position, sphereRadius, 8, 8, BLACK);
      }
      if (selectedSphere >= 0) {
        Vector3 pos = spheres[selectedSphere].position;
        DrawLine3D(pos, Vector3Add(pos, (Vector3){axisLength, 0, 0}), RED);
        DrawLine3D(pos, Vector3Add(pos, (Vector3){0, axisLength, 0}), GREEN);
        DrawLine3D(pos, Vector3Add(pos, (Vector3){0, 0, axisLength}), BLUE);
      }
    }
    for (const auto &connection : connections) {
      Vector3 start = spheres[connection.first].position;
      Vector3 end = spheres[connection.second].position;
      Vector3 step = Vector3Scale(Vector3Subtract(end, start),
                                  1.0f / stepsBetweenConnections);

      for (int i = 0; i <= stepsBetweenConnections; i++) {
        Vector3 cubePos = Vector3Add(start, Vector3Scale(step, i));
        rlPushMatrix();
        rlTranslatef(cubePos.x, cubePos.y, cubePos.z);
        rlRotatef(connection.rotationAngle, 0.0f, 1.0f, 0.0f);
        DrawCube(Vector3Zero(), 0.5f, sphereRadius * 0.5f, 1.0f, GRAY);
        rlPopMatrix();
      }
    }
  }
};
