#pragma once

#include "raylibwrapper.hpp"
enum Tool { NONE, DISTANCE_TOOL, CAMERA_TOOL, CLOCK_TOOL, ADD_CAMERA_TOOL };

class Toolbar {
private:
  std::function<void(std::string)> func;

public:
  static RaylibWrapper *viewer;
  static int state;
  static bool isHidden;

  void init(RaylibWrapper *viewer, std::function<void(std::string)>);
  void render();
  void render_toolbar();
  static void render_distance_tool();
  static void render_camera_tool();
  static void render_add_camera_tool();
  static void set_camera();
  static void render_clock_tool();
};
