#pragma once
#include <iostream>
#include <functional>
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

  Response routeHandler(Request& req)
  {
    std::string header{};
    std::string payload{};
    Response res(header, payload);

    for (const auto& r : m_routes ){
      if (req.m_path == r.path) {
        Response res = r.handler(req);
        return res;
      }
      else {
        std::cerr << "Path: " << req.m_path << " does not exist.\n";
      }
    }
    return res;
  }

};
} // End of namespace

