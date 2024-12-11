#pragma once

#include "raylibwrapper.hpp"
enum Tool { NONE, DISTANCE_TOOL, CAMERA_TOOL, CLOCK_TOOL, ADD_CAMERA_TOOL };

class Toolbar {
public:
  static RaylibWrapper *viewer;
  static int state;
  static bool isHidden;

  static void init(RaylibWrapper *viewer);
  static void render();
  static void render_toolbar();
  static void render_distance_tool();
  static void render_camera_tool();
  static void render_add_camera_tool();
  static void set_camera();
  static void render_clock_tool();
};