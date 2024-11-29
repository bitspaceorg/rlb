#include "include/io.hpp"

using namespace cv;

int main() {
  std::string image_path = "/Users/suryaprakash/Developer/sih/test.jpeg";
  IOHelper *io = new IOHelper();
  auto img = io->read_image(image_path);
  img.water_shed();
  return 0;
}
