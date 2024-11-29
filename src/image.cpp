#include "../include/image.hpp"
#include <opencv2/opencv.hpp>

Image::Image(cv::Mat image) {
	this->image = image;
}

void Image::denoise_image(){
		cv::fastNlMeansDenoisingColored(this->image,this->image,15,15,7,21);
}

void Image::remove_text() {
    cv::Mat gray,thresh,close,dilate;
    cvtColor(this->image, gray, cv::COLOR_BGR2GRAY);

    threshold(gray, thresh, 0, 255, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);
    cv::Mat close_kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 3));
    cv::morphologyEx(thresh, close, cv::MORPH_CLOSE, close_kernel, cv::Point(-1, -1), 1);

    cv::Mat dilate_kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 3));
    cv::dilate(close, dilate, dilate_kernel, cv::Point(-1, -1), 1);

    std::vector<std::vector<cv::Point>> countars;
    cv::findContours(dilate, countars, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (size_t i = 0; i < countars.size(); ++i) {
        double area = cv::contourArea(countars[i]);
        if (area > 800 && area < 15000) {
            cv::Rect bounding_rect = cv::boundingRect(countars[i]);
            rectangle(this->image, bounding_rect, cv::Scalar(222, 228, 251), -1);
        }
    }

    cv::imshow("Result", this->image);
    cv::waitKey(0);
}


void Image::display_image(){
  if (this->image.empty()) {
    std::cout << "Could not read the image: " << std::endl;
		exit(1);
  }
	cv::imshow("Display",this->image);
	cv::waitKey(0);
}

void Image::get_gray_image() {
	cv::cvtColor(this->image,this->image,cv::COLOR_BGR2GRAY);
	cv::threshold(this->image,this->image,160,255,cv::THRESH_BINARY);
	cv::dilate(this->image,this->image,cv::Mat());
	cv::dilate(this->image,this->image,cv::Mat());

	cv::imshow("Gray Image",this->image);
	cv::waitKey(0);
}
void Image::water_shed(std::vector<std::vector<cv::Point>>&contours) {
	cv::Mat thresh,gray;
	this->denoise_image();
	this->get_gray_image();
	cv::Canny(this->image,thresh,100,200);
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(thresh,contours,hierarchy,cv::RETR_TREE,cv::CHAIN_APPROX_SIMPLE);
	cv::Mat countourImage(this->image.size(),CV_8UC3,cv::Scalar(0,0,0));
	cv::Scalar color[3];
	color[0] = cv::Scalar(255,0,0);
	color[1] = cv::Scalar(0,255,0);
	color[2] = cv::Scalar(0,0,255);
	cv::waitKey(0);
}

