#pragma once

#include "raylibwrapper.hpp"
enum Tool {
  NONE,
  DISTANCE_TOOL,
  CAMERA_TOOL,
  CLOCK_TOOL,
  ADD_CAMERA_TOOL,
  EDIT_WALL
};

class Toolbar {
private:
  std::function<void(std::vector<std::string>)> func;

public:
  static RaylibWrapper *viewer;
  static int state;
  static bool isHidden;

  void init(RaylibWrapper *viewer,
            std::function<void(std::vector<std::string>)>);
  void render();
  void render_toolbar();
  static bool isToggleEdit;

  static bool isAddStairPoints;

  static void render_distance_tool();
  static void render_camera_tool();
  static void render_add_camera_tool();
  static void set_camera();
  static void render_clock_tool();
};
