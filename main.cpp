#include <functional>
#include <iostream>
#include "server.hpp"
#include "request.hpp"
#include "response.hpp"
#include "html-render.hpp"

// WARN: This is a usage example file - you are only required to import the header files into your project

// Handler creation example
HTTPResponse index_handler([[maybe_unused] ]HTTPRequest& req)
{
  std::cout << "PATH ==> " << req.m_path << '\n';
  std::cout << "METHOD ==> " << req.m_method << '\n';

  const std::string header       = "HTTP/1.1";
  const std::string payload      = "<h1> Hello, World! </h1>";
  const short status_code        = 200;
  const std::string content_type = "text/html";

  HTTPResponse res(header, payload, content_type, status_code);
  return res;
}

// HTML Rendering example
HTTPResponse home([[maybe_unused]] HTTPRequest& req)
{
  HTMLRender::Renderer render{};
  const std::string file_name    = "index.html";
  const std::string header       = "HTTP/1.1";
  const std::string payload      = render.renderHtml(file_name);
  const short status_code        = 200;
  const std::string content_type = "text/html";

  HTTPResponse res(header, payload, content_type, status_code);
  return res;

}

int main()
{
  // Server creation and route creation
  HTTPServer::Server server(8080, 10);
  server.addRoute("GET", "/index", index_handler);
  server.addRoute("GET", "/home", home);
  server.run();

  return 0;
}
