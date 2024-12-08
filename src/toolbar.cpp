#include "toolbar.hpp"
#include "imgui.h"

int Toolbar::state = Tool::NONE;

inline ImGuiWindowFlags WINDOW_FLAGS =
    0 | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar |
    ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
    ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus;

void Toolbar::render() {
    if (Toolbar::state == Tool::DISTANCE_TOOL) Toolbar::render_distance_tool();
    else if (Toolbar::state == Tool::CAMERA_TOOL) Toolbar::render_camera_tool();
    else if (Toolbar::state == Tool::CLOCK_TOOL) Toolbar::render_clock_tool();

    Toolbar::render_toolbar();
}

void Toolbar::render_distance_tool() {
    const ImGuiViewport *main_viewport = ImGui::GetMainViewport();

    float window_width = 60;
    float window_height = 700;

    ImGui::SetNextWindowPos(ImVec2(main_viewport -> Size.x - 5 - window_width, (main_viewport -> Size.y - window_height) * 0.5f));
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

    float window_width = 60;
    float window_height = 700;

    ImGui::SetNextWindowPos(ImVec2(main_viewport -> Size.x - 5 - window_width, (main_viewport -> Size.y - window_height) * 0.5f));
    ImGui::SetNextWindowSize(ImVec2(60, 700));

    ImGui::Begin("CAMERA", NULL, WINDOW_FLAGS);

    ImGui::Text("CAMERA");
    ImGui::SameLine();

    ImVec2 available_space = ImGui::GetContentRegionAvail();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::PopStyleVar(2);
    ImGui::SameLine();

    ImGui::End();
}

void Toolbar::render_clock_tool() {
    const ImGuiViewport *main_viewport = ImGui::GetMainViewport();

    float window_width = 60;
    float window_height = 700;

    ImGui::SetNextWindowPos(ImVec2(main_viewport -> Size.x - 5 - window_width, (main_viewport -> Size.y - window_height) * 0.5f));
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

    ImGui::SetNextWindowPos(ImVec2(5, (main_viewport -> Size.y - window_height) * 0.5f));
    ImGui::SetNextWindowSize(ImVec2(60, 700));

    ImGui::Begin("TOOLBAR", NULL, WINDOW_FLAGS);

    ImVec2 available_space = ImGui::GetContentRegionAvail();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    // CAMERA TOGGLE
    if (ImGui::Selectable("C", Toolbar::state == Tool::CAMERA_TOOL,
                          ImGuiSelectableFlags_None, ImVec2(40 - 12, 40))) {
        Toolbar::state = Toolbar::state == Tool::CAMERA_TOOL ? Tool::NONE : Tool::CAMERA_TOOL;
    }

    // DISTANCE TOGGLE
    if (ImGui::Selectable("D", Toolbar::state == Tool::DISTANCE_TOOL,
                          ImGuiSelectableFlags_None, ImVec2(40 - 12, 40))) {
        Toolbar::state = Toolbar::state == Tool::DISTANCE_TOOL ? Tool::NONE : Tool::DISTANCE_TOOL;
    }

    // DAY NIGHT TOGGLE
    if (ImGui::Selectable("T", Toolbar::state == Tool::CLOCK_TOOL,
                          ImGuiSelectableFlags_None, ImVec2(40 - 12, 40))) {
        Toolbar::state = Toolbar::state == Tool::CLOCK_TOOL ? Tool::NONE : Tool::CLOCK_TOOL;
    }

    ImGui::PopStyleVar(2);
    ImGui::SameLine();

    ImGui::End();
}
