#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include "image.hpp"

using namespace cv;

class IOHelper {
public:
  Image read_image(std::string);
};
