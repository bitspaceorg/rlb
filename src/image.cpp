#include "../include/image.hpp"
#include <opencv2/opencv.hpp>

Image::Image(cv::Mat image) {
	this->image = image;
}

void Image::denoise_image(){
		cv::fastNlMeansDenoisingColored(this->image,this->image,15,15,7,21);
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

	cv::imshow("Gray Image",this->image);
	cv::waitKey(0);
}
void Image::water_shed(){
	cv::Mat thresh,gray;
	this->denoise_image();
	this->get_gray_image();
	// cv::GaussianBlur(this->image,this->image,cv::Size(5,5),0);
	cv::Canny(this->image,thresh,100,200);

	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	//
	cv::findContours(thresh,contours,hierarchy,cv::RETR_TREE,cv::CHAIN_APPROX_SIMPLE);
	cv::Mat countourImage(this->image.size(),CV_8UC3,cv::Scalar(0,0,0));
	cv::Scalar color[3];
	color[0] = cv::Scalar(255,0,0);
	color[1] = cv::Scalar(0,255,0);
	color[2] = cv::Scalar(0,0,255);
	for (int i=0;i<contours.size();i++) {
		std::cout<< contours[i] << std::endl;
		cv::drawContours(countourImage,contours,i,color[i%3]);
	}
	cv::imshow("Display",countourImage);
	cv::waitKey(0);
}

