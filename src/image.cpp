#include "../include/image.hpp"
#include <opencv2/opencv.hpp>

CustImage::CustImage(cv::Mat image) {
	this->image = image;
}

void CustImage::denoise_image(){
		cv::fastNlMeansDenoisingColored(this->image,this->image,15,15,7,21);
}

void CustImage::remove_text() {
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


void CustImage::display_image(){
  if (this->image.empty()) {
    std::cout << "Could not read the image: " << std::endl;
		exit(1);
  }
	cv::imshow("Display",this->image);
	cv::waitKey(0);
}

void CustImage::get_gray_image() {
	cv::cvtColor(this->image,this->image,cv::COLOR_BGR2GRAY);
    cv::threshold(this->image, this->image, 0, 255, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);
    cv::Mat kernel = cv::Mat::ones(3, 3, CV_8U);
    cv::erode(this->image, this->image, kernel, cv::Point(-1, -1), 2);
    cv::dilate(this->image, this->image, kernel, cv::Point(-1, -1), 2);
    cv::Canny(this->image, this->image, 50, 150);

	cv::imshow("Display",this->image);
	cv::waitKey(0);
}
void CustImage::water_shed(std::vector<std::vector<cv::Point>>&contours) {
	cv::Mat thresh,gray;
	this->denoise_image();
	this->get_gray_image();
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(this->image,contours,hierarchy,cv::RETR_TREE,cv::CHAIN_APPROX_SIMPLE);

	//visualization
	// cv::Mat colorOutput = cv::Mat::zeros(this->image.size(), CV_8UC3);
	// std::cout<<contours.size()<<std::endl;
	// std::srand(time(nullptr));
 //    for (size_t i = 0; i < contours.size(); i++) {
 //        cv::Scalar color(
 //        	rand()%255,
 //        	rand()%255,
 //        	rand()%255
 //        );
 //        cv::drawContours(colorOutput, contours, i, color, 2);
 //    }
	// cv::imshow("Display",colorOutput);
	// cv::waitKey(0);
}

