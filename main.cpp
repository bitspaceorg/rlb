#include "raylibwrapper.hpp"
#include "io.hpp"

using namespace cv;

int main() {
    std::string image_path = "/Users/rahulnavneeth/projects/rlb-core/test4.jpeg";
    IOHelper *io = new IOHelper();
    auto img = io->read_image(image_path);
    std::vector<std::vector<cv::Point2d>> contours;
    img.water_shed(contours);

    RaylibWrapper viewer(800, 600, "3D Room Viewer");
    viewer.init();

    std::vector<cv::Point2d> input_2D;
    for (const auto &points : contours) {
        for (const auto &point : points) {
            input_2D.push_back(point);
        }
    }

    std::vector<cv::Point2d> boundary = viewer.get_bounding_box(input_2D);

    Shader aoShader = LoadShader("./src/ambient_occlusion.vs", "ambient_occlusion.fs");
    SetShaderValue(aoShader, GetShaderLocation(aoShader, "lightDir"), (float[3]){0.0f, 1.0f, 0.0f}, SHADER_UNIFORM_VEC3);
    SetShaderValue(aoShader, GetShaderLocation(aoShader, "ambientStrength"), (float[1]){0.5f}, SHADER_UNIFORM_FLOAT);

    while (!WindowShouldClose()) {
        viewer.update_camera();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(viewer.get_camera());
        DrawGrid(10, 2.0f);

        BeginShaderMode(aoShader);

        	viewer.render(contours, viewer.colors[5]);
        	viewer.render_base(boundary, 0.0f, viewer.colors[5]);

        EndShaderMode();

        EndMode3D();
        EndDrawing();
    }

    UnloadShader(aoShader);
    delete io;
    return 0;
}
