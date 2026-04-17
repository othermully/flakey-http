#pragma once
#include <iostream>
#include <functional>
#include <stdexcept>
#include <vector>
#include "request.hpp"
#include "response.hpp"

// TODO: If javascript and css are requested on the page, the router should be able to send those when requested

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
    std::string header{ "HTTP/1.1" };
    std::string payload{ "TESTING" };
    short status_code { 200 };
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

};
} // End of namespace

