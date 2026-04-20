#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <vector>
#include "request.hpp"
#include "response.hpp"

namespace HTTPRouter{

using Request = HTTPRequest;
using Response = HTTPResponse;

class Router{
public:
  struct Route{
    std::string method{};
    std::string path{};
    std::function<Response(Request&)> handler;
  };

  std::vector<Route> m_routes{};

  void addRoute(const std::string& method, const std::string& path,
                 std::function<Response(Request&)> handler)
  {
    if (path[0] != '/') {
      throw std::runtime_error("Path must begin with a /. (e.g. /get_users)");
    }

    Route route{};
    route.method = method;
    route.path = path;
    route.handler = handler;
    m_routes.push_back(route);
    std::cout << "Route: " << route.method << ' ' << route.path << " added.\n";
  }

  // TODO: Maybe here we can check if client is requesting .js or .css files, if path contains .css or .js
  Response routeHandler(Request& req)
  {

    if (getExtension(req.m_path) == ".js") {
      std::string header { "HTTP/1.1" };
      std::string payload = getStaticFile(req.m_path);
      short status_code { 200 };
      std::string content_type { "text/javascript" };

      Response res(header, payload, content_type, status_code);
      return res;
    }

    if (getExtension(req.m_path) == ".css") {
      std::string header { "HTTP/1.1" };
      std::string payload = getStaticFile(req.m_path);
      short status_code { 200 };
      std::string content_type { "text/css" };

      Response res(header, payload, content_type, status_code);
      return res;
    }

    // Building default response if route not found.
    std::string header{ "HTTP/1.1" };
    std::string payload{ "Not Found" };
    short status_code { 404 };
    std::string content_type{ "text/html" };
    Response res(header, payload, content_type, status_code);

    for (const auto& r : m_routes ){
      if (req.m_path == r.path) {
        Response res = r.handler(req);
        return res;
      }
      else {
        continue;
      }
    }
    return res;
  }

private:

  static std::string getExtension(const std::string& path)
  {
    size_t pos = path.find_last_of('.');
    if (pos == std::string::npos) return "";
    return path.substr(pos);
  }

  // Idk where to put this yet
  static std::string getStaticFile(const std::string& file_name)
  {
    std::string path = "./static/" + file_name;
    if (std::filesystem::exists(path)) {
      std::ifstream ifs(path);
      std::string static_file ( (std::istream_iterator<char>(ifs) ), ( std::istream_iterator<char>() ) );
      return static_file;
    }
    return "";
  }

};
} // End of namespace

