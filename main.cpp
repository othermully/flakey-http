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

  std::string header = "TESTING";
  std::string payload = "THIS IS A PAYLOAD";
  HTTPResponse res(header, payload);
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
