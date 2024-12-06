#include "io.hpp"
#include <iostream>
#include <filesystem>

//returns a image class
CustImage IOHelper::read_image(std::string path) {
  Mat image = imread(path, IMREAD_COLOR);
	std::cout<<std::filesystem::current_path()<<"\n";
  if (image.empty()) {
		std::cout<<path<<"\n";
    std::cerr << "[ERROR] CustImage Not Found in Path!";
    std::exit(1);
  }
    return CustImage(image);
}
