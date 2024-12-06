#pragma once
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/photo.hpp>

class CustImage{
private:
	cv::Mat image;
public:
	CustImage(cv::Mat);
	void display_image();

  // returns a image
  void denoise_image();
  void get_gray_image();
  void remove_text();
  void water_shed(std::vector<std::vector<cv::Point2d>>&);
  void normalize(std::vector<std::vector<cv::Point>>&, std::vector<std::vector<cv::Point2d>>&);
};
