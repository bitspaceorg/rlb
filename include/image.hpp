#pragma once
#include<opencv2/core.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/photo.hpp>

class Image {
private:
	cv::Mat image;
public:
	Image(cv::Mat);
	void display_image();

	//returns a image
	void denoise_image();
	void get_gray_image();
	void water_shed();
};
