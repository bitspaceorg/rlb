#include "api.hpp"

array<vector<array<ApiPoint, 2>>, 3> API::post(std::string path) {
    try {
        boost::asio::io_context io;

        boost::asio::ip::tcp::resolver resolver(io);
        boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "5500");
        boost::asio::ip::tcp::socket socket(io);
        boost::asio::connect(socket, endpoints);

        std::string post_data = nlohmann::json{{"path", path}}.dump();
        std::string request =
            "POST / HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: " + std::to_string(post_data.size()) + "\r\n"
            "Connection: close\r\n\r\n" +
            post_data;

        boost::asio::write(socket, boost::asio::buffer(request));

        boost::asio::streambuf response_buffer;
        boost::asio::read_until(socket, response_buffer, "\r\n");

        std::istream response_stream(&response_buffer);
        std::string http_version;
        unsigned int status_code;
        std::string status_message;

        response_stream >> http_version >> status_code;
        std::getline(response_stream, status_message);

        if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
            std::cerr << "Invalid response\n";
            return {};
        }
        if (status_code != 200) {
            std::cerr << "Response returned with status code " << status_code << "\n";
        }

        std::string header;

        std::ostringstream response_body;
        if (response_buffer.size() > 0) {
            response_body << &response_buffer;
        }

        boost::system::error_code ec;
        boost::asio::read(socket, response_buffer, boost::asio::transfer_all(), ec);

        if (ec && ec != boost::asio::error::eof) {
            std::cerr << "Read error: " << ec.message() << "\n";
            return {};
        }

        response_body << &response_buffer;
        
        std::string raw_response = response_body.str();
        std::size_t body_start_pos = raw_response.find("\r\n\r\n");
        
        if (body_start_pos == std::string::npos) {
            std::cerr << "Error: No valid body found in the response.\n";
            return {};
        }
        
        std::string json_body = raw_response.substr(body_start_pos + 4);
        
        std::cout << "Response Body (JSON):\n" << json_body << std::endl;

        nlohmann::json j = nlohmann::json::parse(json_body);
        array<vector<array<ApiPoint, 2>>, 3> ret;

        for (auto &coord : j["wall"]) 
            ret[0].push_back({ApiPoint(coord["x1"], coord["y1"], 0.0f), ApiPoint(coord["x2"], coord["y2"], 0.0f)});

        for (auto &coord : j["window"]) 
            ret[1].push_back({ApiPoint(coord["x1"], coord["y1"], 0.0f), ApiPoint(coord["x2"], coord["y2"], 0.0f)});

        for (auto &coord : j["door"]) 
            ret[2].push_back({ApiPoint(coord["x1"], coord["y1"], 0.0f), ApiPoint(coord["x2"], coord["y2"], 0.0f)});

        return ret;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        return {};
    }
}
