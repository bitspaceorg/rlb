#include "io.hpp"
#include "api.hpp"
#include <opencv2/core/types.hpp>

namespace fs = std::filesystem;

// returns a image class
// std::vector<std::vector<cv::Point2d>> contours2d;
void IOHelper::read_image(std::string path,
                          std::vector<std::vector<cv::Point>> &contour,int index) {

	std::filesystem::path p = path;
  auto data = API::post(fs::absolute(p));

  // process walls
  for (auto x : data[index]) {
    auto a = x[0];
    auto b = x[1];
    vector<cv::Point> temp;
    temp.emplace_back(a.x, a.y);
    temp.emplace_back(b.x, a.y);
    temp.emplace_back(b.x, b.y);
    temp.emplace_back(a.x, b.y);
		
		contour.push_back(temp);
  }
}
