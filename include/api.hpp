#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

using boost::asio::ip::tcp;
using namespace std;

struct ApiPoint {
public:
    float x, y, z;
    ApiPoint(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
};

class API {
    public:
        static array<vector<array<ApiPoint, 2>>, 3> post(std::string path);
};

#endif
