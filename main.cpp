#include <functional>
#include <iostream>
#include "server.hpp"
#include "request.hpp"
#include "response.hpp"


void index_handler([[maybe_unused]]HTTPRequest& req, [[maybe_unused]]HTTPResponse& res){
  std::cout << "Running idex_handler.\n";
}

int main(){
  int port    { 8080 };
  int backlog { 10 };

  HTTPServer::Server server(port, backlog);
  server.addRoute("GET", "/index", index_handler);
  server.run();


  return 0;
}
