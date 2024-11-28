#include "../include/io.hpp"
#include<iostream>

//returns a image class
Image IOHelper::read_image(std::string path) {
  Mat image = imread(path, IMREAD_COLOR);

  if (image.empty()) {
		std::cout<<path<<"\n";
    std::cerr << "[ERROR] Image Not Found in Path!";
    std::exit(1);
  }
    return Image(image);
}