#include <functional>
#include <iostream>
#include "server.hpp"
#include "request.hpp"
#include "response.hpp"


// Handler creation example
HTTPResponse index_handler([[maybe_unused]]HTTPRequest& req)
{
  std::cout << "Running index_handler.\n";
  std::cout << "PATH ==> " << req.m_path << '\n';
  std::cout << "METHOD ==> " << req.m_method << '\n';

  std::string header = "HTTP/1.1";
  std::string payload = "<h1> Hello, World! </h1>";
  short status_code = 200;
  std::string content_type = "text/html";
  HTTPResponse res(header, payload, content_type, status_code);
  return res;
}

int main()
{
  int port    { 8080 };
  int backlog { 10 };

  HTTPServer::Server server(port, backlog);
  server.addRoute("GET", "/index", index_handler);
  server.run();


  return 0;
}
