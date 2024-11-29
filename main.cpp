#include "./include/io.hpp"
#include "./include/RaylibWrapper.hpp"

using namespace cv;

int main() {

  std::string image_path = "/Users/rahulnavneeth/projects/rlb-core/test.jpeg";
  IOHelper *io = new IOHelper();
  auto img = io->read_image(image_path);
  std::vector<std::vector<cv::Point>> contours;
  img.water_shed(contours);

  RaylibWrapper viewer(800, 600, "3D Room Viewer");
  viewer.init();

  while (!WindowShouldClose()) {
      viewer.update_camera();
      viewer.render(contours);
  }

  return 0;
}
