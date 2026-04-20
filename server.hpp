/*
 HTTP Server
 Constructed with a port and backlog
 start the server with server.run()
*/

#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <functional>
#include "json.hpp"
#include "response.hpp"
#include "request.hpp"
#include "http-parser.hpp"
#include "http-router.hpp"


namespace HTTPServer{
using Request  = HTTPRequest;
using Response = HTTPResponse;

class Server{
public:
  int m_sockfd{};
  int m_port{};
  sockaddr_in m_server_addr{};
  int m_backlog{}; 

  HTTPParser::Parser http_parser{};
  HTTPRouter::Router http_router{};
  JSONParser::Parser json_parser{};

  // Constructor
  Server(int port, int backlog){
    m_port    = port;
    m_backlog = backlog;
  };


  void addRoute(const std::string method, const std::string path, std::function<Response(Request&)> handler)
  {
    http_router.addRoute(method, path, handler);
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
      std::cout << "Received from client: \n" << buffer << '\n';
      Request req = http_parser.parseRequest(buffer); // Pass this request to middleware loop to enrich/transform it
                                                 // When ready, the handler that the req was sent to will create a response
                                                 // The server will serialize that response and send back to the client

      // TODO: This should be handled in the parser, i think
     // if (!req.m_body.empty()) {
     //   // WARN: Assuming JSON only in body
     //   std::cout << "Req contains JSON\n";
     //   size_t i = 0;
     //   JSONParser::JSON json = json_parser.parseValue(req.m_body, i) ;
     //    req.m_json = json;
     // }

      // Generate response, serialize it, send it, done
      Response res = http_router.routeHandler(req); // This router should dispatch the request to the correct handler
      std::string response_payload = serializeResponse(res);
      send(client_fd, response_payload.c_str(), response_payload.size(), 0);

      // DEBUG
      std::cout << "Method ==> "     << req.m_method << '\n';
      std::cout << "Path ==> "       << req.m_path << '\n';
      std::cout << "Host ==> "       << req.m_host << '\n';
      std::cout << "User-Agent ==> " << req.m_user_agent << '\n';

      close(client_fd);
    }

  }

  int getServerFd()
  {
    uint16_t port                 = static_cast<uint16_t>(m_port);
    int server_socket             = socket(AF_INET, SOCK_STREAM, 0);
    m_server_addr.sin_family      = AF_INET;
    m_server_addr.sin_port        = htons(port);
    m_server_addr.sin_addr.s_addr = INADDR_ANY;

    return server_socket;
  }

  std::string serializeResponse(Response& res)
  {
    std::string temp{};
    std::cout << "Serializing response...\n";

    // Header example: HTTP/1.1 - technically this is a status line, will work on this later, maybe
    // TODO: Status code should resolve to string (e.g. 200 = "200 OK")
    // TODO: Need some kind of HTML rendering function that serializes HTML, JS and CSS to send as payload when requested
    if (res.m_status_code == 404) {
      temp = res.m_header + " " + std::to_string(res.m_status_code) + " Not Found\r\n" + res.m_content_type + "\r\n\r\n\n";
    }
    temp = res.m_header + " " + std::to_string(res.m_status_code) + " OK\r\n" + res.m_content_type + "\r\n\r\n\n" + res.m_payload;
    return temp;
  }

};

} // End of namespace


