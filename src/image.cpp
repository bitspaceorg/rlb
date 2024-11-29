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

    // cv::imshow("Result", this->image);
    // cv::waitKey(0);
}


void CustImage::display_image(){
  if (this->image.empty()) {
    std::cout << "Could not read the image: " << std::endl;
		exit(1);
  }
	// cv::imshow("Display",this->image);
	// cv::waitKey(0);
}

void CustImage::get_gray_image() {
	cv::cvtColor(this->image,this->image,cv::COLOR_BGR2GRAY);
	cv::threshold(this->image,this->image,160,255,cv::THRESH_BINARY);
	cv::dilate(this->image,this->image,cv::Mat());
	cv::dilate(this->image,this->image,cv::Mat());
}

void CustImage::water_shed(std::vector<std::vector<cv::Point2d>>&contours2d) {
	cv::Mat thresh,gray;
	this->denoise_image();
	this->get_gray_image();
	cv::Canny(this->image,thresh,100,200);
	std::vector<cv::Vec4i> hierarchy;
	std::vector<std::vector<cv::Point>>contours;
	cv::findContours(thresh,contours,hierarchy,cv::RETR_TREE,cv::CHAIN_APPROX_SIMPLE);
	this->normalize(contours, contours2d);
}

void CustImage::normalize(std::vector<std::vector<cv::Point>>& contours, std::vector<std::vector<cv::Point2d>>& contours2d) {
    contours2d.resize(contours.size());
    int xMax, yMax, xMin, yMin;
    xMax = yMax = 0;
    xMin = yMin = INT_MAX;

    for (auto& contour : contours) {
        for (auto& P : contour) {
            xMax = std::max(xMax, P.x);
            yMax = std::max(yMax, P.y);
            xMin = std::min(xMin, P.x);
            yMin = std::min(yMin, P.y);
        }
    }

    int deltaX = xMax - xMin;
    int deltaY = yMax - yMin;

    const int TARGET_SIZE = 100;

    if (deltaX > deltaY) { // ALONG X
        double scaleX = static_cast<double>(TARGET_SIZE) / deltaX;
        
        for (size_t i = 0; i < contours.size(); ++i) {
            contours2d[i].resize(contours[i].size());
            for (size_t j = 0; j < contours[i].size(); ++j) {
                contours2d[i][j].x = (contours[i][j].x - xMin) * scaleX;
                contours2d[i][j].y = (contours[i][j].y - yMin) * scaleX;
            }
        }
    } else { // ALONG Y
        double scaleY = static_cast<double>(TARGET_SIZE) / deltaY;
        
        for (size_t i = 0; i < contours.size(); ++i) {
            contours2d[i].resize(contours[i].size());
            for (size_t j = 0; j < contours[i].size(); ++j) {
                contours2d[i][j].x = (contours[i][j].x - xMin) * scaleY;
                contours2d[i][j].y = (contours[i][j].y - yMin) * scaleY;
            }
        }
    }
}
