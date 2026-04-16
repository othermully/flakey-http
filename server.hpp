/*
 HTTP Server
 Constructed with a port and backlog
 start the server with server.startServer()
*/

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <vector>
#include <functional>
#include "response.hpp"
#include "request.hpp"
#include "http-parser.hpp"


namespace HTTPServer{
using Request  = HTTPRequest;
using Response = HTTPResponse;

class Server{
public:
  struct Route{
    std::string method{};
    std::string path{};
    std::function<void(Request&, Response&)> handler;
  };
  std::vector<Route> routes{};

  int m_sockfd{};
  int m_port{};
  sockaddr_in m_server_addr{};
  int m_backlog{}; 
  HTTPParser::Parser parser{};

  // Constructor
  Server(int port, int backlog){
    m_port    = port;
    m_backlog = backlog;
  };


  void addRoute(const std::string method, const std::string path, std::function<void(Request&, Response&)> handler)
  {
    if (path[0] != '/') {
      throw std::runtime_error("Path must begin with a /. (e.g. /get_users)");
    }

    Route route{};
    route.method  = method;
    route.path    = path;
    route.handler = handler;
    routes.push_back(route);
  }

  void run()
  {
    m_sockfd = getServerFd();

    if ((bind(m_sockfd, (struct sockaddr*)&m_server_addr, sizeof(m_server_addr))) == -1) {
      std::cerr << "Failed to bind. Most likely address reuse.\n";
      exit(1);
    }
    if ((listen(m_sockfd, m_backlog)) == -1) {
      std::cerr << "Failed to listen.\n";
      exit(1);
    }
    std::cout << "Listening on port " << m_port << "...\n";
    acceptConnections();
  }

private:
  void acceptConnections()
  {
    while (true) {
      struct sockaddr_in client_addr;
      socklen_t client_addr_len = sizeof(client_addr);
      int client_fd = accept(m_sockfd, (struct sockaddr*)&client_addr, &client_addr_len);

      char buffer[1024];
      ssize_t bytes_received = recv(client_fd, &buffer, sizeof(buffer), 0);

      if (bytes_received == 0) {
        std::cerr << "Lost client connection.";
        close(client_fd);
      }

      // Build the request object -> Middleware + Routing + Handler
      std::cout << "Received from client: \n" << buffer;
      Request req = parser.parseRequest(buffer); // Pass this request to middleware loop to enrich/transform it
                                                 // When ready, the handler that the req was sent to will create a response
                                                 // The server will serialize that response and send back to the client

      std::cout << "Method ==> "     <<  req.m_method << '\n';
      std::cout << "Path ==> "       << req.m_path << '\n';
      std::cout << "Host ==> "       << req.m_host << '\n';
      std::cout << "User-Agent ==> " << req.m_user_agent << '\n';
    }

  }

  int getServerFd()
  {
    uint16_t port = static_cast<uint16_t>(m_port);
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_port = htons(port);
    m_server_addr.sin_addr.s_addr = INADDR_ANY;
    return server_socket;
  }
};

} // End of namespace


