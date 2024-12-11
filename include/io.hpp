#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace cv;

class IOHelper {
public:
  void read_image(std::string, std::vector<std::vector<cv::Point>> &);
};
