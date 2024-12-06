#pragma once
#include "image.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace cv;

class IOHelper {
public:
  CustImage read_image(std::string);
};
