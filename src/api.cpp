#include "api.hpp"

array<vector<array<ApiPoint, 2>>, 3> API::post(std::string path) {
    // try {
    //     boost::asio::io_context io;

    //     boost::asio::ip::tcp::resolver resolver(io);
    //     boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve("localhost", "8000");
    //     boost::asio::ip::tcp::socket socket(io);
    //     boost::asio::connect(socket, endpoints);

    //     std::string post_data = "path=" + path;
    //     std::string request =
    //         "POST / HTTP/1.1\r\n"
    //         "Host: localhost\r\n"
    //         "Content-Type: application/x-www-form-urlencoded\r\n"
    //         "Content-Length: " + std::to_string(post_data.size()) + "\r\n"
    //         "Connection: close\r\n\r\n" +
    //         post_data;

    //     boost::asio::write(socket, boost::asio::buffer(request));

    //     boost::asio::streambuf response_buffer;
    //     boost::asio::read_until(socket, response_buffer, "\r\n");

    //     std::istream response_stream(&response_buffer);
    //     std::string http_version;
    //     unsigned int status_code;
    //     std::string status_message;

    //     response_stream >> http_version >> status_code;
    //     std::getline(response_stream, status_message);

    //     if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
    //         std::cerr << "Invalid response\n";
    //         return {};
    //     }
    //     if (status_code != 200) {
    //         std::cerr << "Response returned with status code " << status_code << "\n";
    //     }

    //     std::string header;

    //     std::ostringstream response_body;
    //     if (response_buffer.size() > 0) {
    //         response_body << &response_buffer;
    //     }

    //     boost::system::error_code ec;
    //     boost::asio::read(socket, response_buffer, boost::asio::transfer_all(), ec);

    //     if (ec && ec != boost::asio::error::eof) {
    //         std::cerr << "Read error: " << ec.message() << "\n";
    //         return {};
    //     }

        // response_body << &response_buffer;

        std::string json_data = R"(
        {
            "Height": 611,
            "Width": 1261,
            "door": [
                {
                    "x1": 221,
                    "x2": 254,
                    "y1": 314,
                    "y2": 374
                },
                {
                    "x1": 575,
                    "x2": 628,
                    "y1": 236,
                    "y2": 244
                },
                {
                    "x1": 747,
                    "x2": 804,
                    "y1": 235,
                    "y2": 243
                },
                {
                    "x1": 1048,
                    "x2": 1064,
                    "y1": 255,
                    "y2": 318
                },
                {
                    "x1": 436,
                    "x2": 491,
                    "y1": 297,
                    "y2": 304
                }
            ],
            "wall": [
                {
                    "x1": 547,
                    "x2": 609,
                    "y1": 541,
                    "y2": 569
                },
                {
                    "x1": 861,
                    "x2": 881,
                    "y1": 78,
                    "y2": 243
                },
                {
                    "x1": 538,
                    "x2": 559,
                    "y1": 82,
                    "y2": 238
                },
                {
                    "x1": 427,
                    "x2": 488,
                    "y1": 540,
                    "y2": 570
                },
                {
                    "x1": 974,
                    "x2": 1062,
                    "y1": 540,
                    "y2": 569
                },
                {
                    "x1": 730,
                    "x2": 873,
                    "y1": 540,
                    "y2": 569
                },
                {
                    "x1": 222,
                    "x2": 251,
                    "y1": 228,
                    "y2": 311
                },
                {
                    "x1": 223,
                    "x2": 305,
                    "y1": 541,
                    "y2": 569
                },
                {
                    "x1": 626,
                    "x2": 744,
                    "y1": 236,
                    "y2": 244
                },
                {
                    "x1": 221,
                    "x2": 253,
                    "y1": 70,
                    "y2": 167
                },
                {
                    "x1": 864,
                    "x2": 1067,
                    "y1": 224,
                    "y2": 244
                },
                {
                    "x1": 491,
                    "x2": 546,
                    "y1": 297,
                    "y2": 304
                },
                {
                    "x1": 768,
                    "x2": 903,
                    "y1": 366,
                    "y2": 385
                },
                {
                    "x1": 659,
                    "x2": 667,
                    "y1": 77,
                    "y2": 239
                },
                {
                    "x1": 973,
                    "x2": 1062,
                    "y1": 366,
                    "y2": 373
                },
                {
                    "x1": 1045,
                    "x2": 1060,
                    "y1": 353,
                    "y2": 566
                },
                {
                    "x1": 538,
                    "x2": 573,
                    "y1": 235,
                    "y2": 244
                },
                {
                    "x1": 246,
                    "x2": 438,
                    "y1": 297,
                    "y2": 305
                },
                {
                    "x1": 803,
                    "x2": 867,
                    "y1": 235,
                    "y2": 244
                },
                {
                    "x1": 223,
                    "x2": 251,
                    "y1": 520,
                    "y2": 568
                },
                {
                    "x1": 1047,
                    "x2": 1066,
                    "y1": 224,
                    "y2": 253
                },
                {
                    "x1": 221,
                    "x2": 251,
                    "y1": 376,
                    "y2": 398
                },
                {
                    "x1": 1046,
                    "x2": 1066,
                    "y1": 318,
                    "y2": 370
                },
                {
                    "x1": 538,
                    "x2": 546,
                    "y1": 242,
                    "y2": 304
                },
                {
                    "x1": 222,
                    "x2": 259,
                    "y1": 69,
                    "y2": 97
                },
                {
                    "x1": 899,
                    "x2": 906,
                    "y1": 325,
                    "y2": 376
                },
                {
                    "x1": 92,
                    "x2": 229,
                    "y1": 256,
                    "y2": 264
                }
            ],
            "window": [
                {
                    "x1": 221,
                    "x2": 252,
                    "y1": 168,
                    "y2": 229
                },
                {
                    "x1": 609,
                    "x2": 731,
                    "y1": 540,
                    "y2": 570
                },
                {
                    "x1": 874,
                    "x2": 972,
                    "y1": 539,
                    "y2": 568
                },
                {
                    "x1": 310,
                    "x2": 428,
                    "y1": 540,
                    "y2": 570
                },
                {
                    "x1": 221,
                    "x2": 251,
                    "y1": 402,
                    "y2": 518
                },
                {
                    "x1": 488,
                    "x2": 548,
                    "y1": 541,
                    "y2": 569
                },
                {
                    "x1": 258,
                    "x2": 321,
                    "y1": 69,
                    "y2": 97
                }
            ]
        }
        )";

        nlohmann::json j = nlohmann::json::parse(json_data);

        array<vector<array<ApiPoint, 2>>, 3> ret;

        for (auto &coord : j["wall"]) 
            ret[0].push_back({ApiPoint(coord["x1"], coord["y1"], 0.0f), ApiPoint(coord["x2"], coord["y2"], 0.0f)});

        for (auto &coord : j["window"]) 
            ret[1].push_back({ApiPoint(coord["x1"], coord["y1"], 0.0f), ApiPoint(coord["x2"], coord["y2"], 0.0f)});

        for (auto &coord : j["door"]) 
            ret[2].push_back({ApiPoint(coord["x1"], coord["y1"], 0.0f), ApiPoint(coord["x2"], coord["y2"], 0.0f)});

        return ret;
    // } catch (const std::exception& e) {
    //     std::cerr << "Exception: " << e.what() << "\n";
    //     return {};
    // }
}
