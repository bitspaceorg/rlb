#pragma once
#include "imgui.h"

enum Tool{ NONE, DISTANCE_TOOL, CAMERA_TOOL, CLOCK_TOOL };

class Toolbar {
    public:
        static int state;

        static void render();
        static void render_toolbar();
        static void render_distance_tool();
        static void render_camera_tool();
        static void render_clock_tool();
};
