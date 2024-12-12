#include "toolbar.hpp"
#include "imgui.h"
#include "raylib.h"
#include "raylibwrapper.hpp"
#include "tinyfiledialogs.h"
#include <functional>
#include <iostream>

int Toolbar::state = Tool::NONE;
RaylibWrapper *Toolbar::viewer = nullptr;
bool Toolbar::isHidden = 0;
bool Toolbar::isToggleEdit = 0;
bool Toolbar::isAddStairPoints = 0;
static bool isAdjusted = 0;

inline ImGuiWindowFlags WINDOW_FLAGS =
    0 | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar |
    ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
    ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus;

void Toolbar::init(RaylibWrapper *initial_viewer,
                   std::function<void(std::vector<std::string>)> func) {
  viewer = initial_viewer;
  this->func = func;
}

void Toolbar::render() {
  if (isHidden && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    isHidden = 0;
    isAdjusted = 1;
    set_camera();
  }
  if (isHidden)
    return;
  if (Toolbar::state == Tool::DISTANCE_TOOL)
    Toolbar::render_distance_tool();
  else if (Toolbar::state == Tool::CAMERA_TOOL)
    Toolbar::render_camera_tool();
  else if (Toolbar::state == Tool::CLOCK_TOOL)
    Toolbar::render_clock_tool();
  else if (Toolbar::state == Tool::ADD_CAMERA_TOOL)
    Toolbar::render_add_camera_tool();

  Toolbar::render_toolbar();

  DrawTextEx(viewer->font,
             TextFormat("DISTANCE FROM THE CAMERA TO THE TARGET: %08f",
                        viewer->distance_from_camera * 3.28084),
             Vector2{30, 30}, 20, 0, RED);

  DrawTextEx(viewer->font,
             TextFormat("%f x %f x %f", viewer->current_dimensions.x,
                        viewer->current_dimensions.y,
                        viewer->current_dimensions.z),
             Vector2{30, 50}, 20, 0, RED);
}

void Toolbar::render_distance_tool() {
  const ImGuiViewport *main_viewport = ImGui::GetMainViewport();

  float window_width = 60;
  float window_height = 700;

  ImGui::SetNextWindowPos(
      ImVec2(main_viewport->Size.x - 5 - window_width,
             (main_viewport->Size.y - window_height) * 0.5f));
  ImGui::SetNextWindowSize(ImVec2(60, 700));

  ImGui::Begin("DISTANCE", NULL, WINDOW_FLAGS);

  ImGui::Text("DISTANCE");
  ImGui::SameLine();

  ImVec2 available_space = ImGui::GetContentRegionAvail();

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

  ImGui::PopStyleVar(2);
  ImGui::SameLine();

  ImGui::End();
}

void Toolbar::render_camera_tool() {
  const ImGuiViewport *main_viewport = ImGui::GetMainViewport();

  float window_width = 80;
  float window_height = 700;

  ImGui::SetNextWindowPos(
      ImVec2(main_viewport->Size.x - 5 - window_width,
             (main_viewport->Size.y - window_height) * 0.5f));
  ImGui::SetNextWindowSize(ImVec2(window_width, window_height));

  ImGui::Begin("CAMERA", NULL, WINDOW_FLAGS);

  ImGui::Text("CAMERA");
  // ImGui::SameLine();

  ImVec2 available_space = ImGui::GetContentRegionAvail();

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

  if (ImGui::Button("+/Add")) {
    ImGui::OpenPopup("ADD CAMERA");
  }
  render_add_camera_tool();

  if (ImGui::Button("*/update")) {
  }

  ImGui::PopStyleVar(2);
  // ImGui::SameLine();

  ImGui::End();
}

void Toolbar::set_camera() {
  ImGui::OpenPopup("ADD CAMERA");
  render_add_camera_tool();
}

void Toolbar::render_add_camera_tool() {
  if (ImGui::BeginPopupModal("ADD CAMERA", NULL,
                             ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2, 0));

    static const int current_mode = viewer->get_mode();

    static const char *cam_type_options[]{"STATIC", "TARGET", "FREE"};
    static int selected_cam_type = 0;

    bool cam_type_check =

        ImGui::Combo("CAMERA TYPE", &selected_cam_type, cam_type_options,
                     IM_ARRAYSIZE(cam_type_options));
    if (cam_type_check) {
      std::cout << "selected? " << selected_cam_type << "\n";
    }

    if (selected_cam_type != 2) {
      if (ImGui::Button("ADJUST CAMERA")) {
        viewer->set_mode(CAMERA_FREE);
        isHidden = 1;
        isAdjusted = 0;
        ImGui::CloseCurrentPopup();
      }
    }

    static float fov = 90.0f;
    static const float current_fov = viewer->get_fov();
    if (ImGui::SliderFloat("FOV", &fov, 30.0f, 120.0f)) {
      viewer->set_fov(fov);
    }

    if (isAdjusted) {
      viewer->set_mode(CAMERA_CUSTOM);
    }

    if (ImGui::Button("OK") && isAdjusted) {
      viewer->add_camera(viewer->get_position(), viewer->get_target(), fov,
                         CAMERA_PERSPECTIVE,
                         selected_cam_type == 0   ? CAMERA_CUSTOM
                         : selected_cam_type == 1 ? CAMERA_FIRST_PERSON
                                                  : CAMERA_TOOL,
                         selected_cam_type == 1 ? 1 : 0);
      viewer->set_fov(current_fov);
      viewer->set_mode(current_mode);
      ImGui::CloseCurrentPopup();
    } else {
      ;
    }
    ImGui::SameLine();
    if (ImGui::Button("CANCEL")) {
      viewer->set_fov(current_fov);
      viewer->set_mode(current_mode);
      ImGui::CloseCurrentPopup();
    }

    ImGui::PopStyleVar();
    ImGui::EndPopup();
  }
}

void Toolbar::render_clock_tool() {
  const ImGuiViewport *main_viewport = ImGui::GetMainViewport();

  float window_width = 60;
  float window_height = 700;

  ImGui::SetNextWindowPos(
      ImVec2(main_viewport->Size.x - 5 - window_width,
             (main_viewport->Size.y - window_height) * 0.5f));
  ImGui::SetNextWindowSize(ImVec2(60, 700));

  ImGui::Begin("CLOCK", NULL, WINDOW_FLAGS);

  ImGui::Text("CLOCK");
  ImGui::SameLine();

  ImVec2 available_space = ImGui::GetContentRegionAvail();

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

  ImGui::PopStyleVar(2);
  ImGui::SameLine();

  ImGui::End();
}

void Toolbar::render_toolbar() {
  const ImGuiViewport *main_viewport = ImGui::GetMainViewport();

  float window_width = 60;
  float window_height = 700;

  ImGui::SetNextWindowPos(
      ImVec2(5, (main_viewport->Size.y - window_height) * 0.5f));
  ImGui::SetNextWindowSize(ImVec2(60, 700));

  ImGui::Begin("TOOLBAR", NULL, WINDOW_FLAGS);

  ImVec2 available_space = ImGui::GetContentRegionAvail();

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

  // CAMERA TOGGLE
  if (ImGui::Selectable("C", Toolbar::state == Tool::CAMERA_TOOL,
                        ImGuiSelectableFlags_None, ImVec2(40 - 12, 40))) {
    Toolbar::state =
        Toolbar::state == Tool::CAMERA_TOOL ? Tool::NONE : Tool::CAMERA_TOOL;
  }

  // DISTANCE TOGGLE
  if (ImGui::Selectable("D", Toolbar::state == Tool::DISTANCE_TOOL,
                        ImGuiSelectableFlags_None, ImVec2(40 - 12, 40))) {
    Toolbar::state = Toolbar::state == Tool::DISTANCE_TOOL
                         ? Tool::NONE
                         : Tool::DISTANCE_TOOL;
  }

  // DAY NIGHT TOGGLE
  if (ImGui::Selectable("T", Toolbar::state == Tool::CLOCK_TOOL,
                        ImGuiSelectableFlags_None, ImVec2(40 - 12, 40))) {
    Toolbar::state =
        Toolbar::state == Tool::CLOCK_TOOL ? Tool::NONE : Tool::CLOCK_TOOL;
  }

  if (ImGui::Button("Upload")) {
    auto name =
        tinyfd_openFileDialog("Choose floor plan", "", 0, NULL, NULL, 1);

    std::stringstream ss(name);
    std::string word;
    std::vector<std::string> splits;
    while (std::getline(ss, word, '|')) {
      splits.push_back(word);
    }
    this->func(splits);
  }
  if (ImGui::Selectable("Edit height", isToggleEdit, ImGuiSelectableFlags_None,
                        ImVec2())) {
    isToggleEdit = !isToggleEdit;
    viewer->is_petrude = isToggleEdit;
  }

  if (ImGui::Selectable("Add stair points", isAddStairPoints,
                        ImGuiSelectableFlags_None, ImVec2())) {
    isAddStairPoints = !isAddStairPoints;
    viewer->is_add_stair_points = isAddStairPoints;
  }

  ImGui::PopStyleVar(2);
  ImGui::SameLine();

  ImGui::End();
}
