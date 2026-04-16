# **Flakey HTTP**

## **Overview**
Flakey HTTP is a Lightweight HTTP server with JSON support and URL routing. **DO NOT** use in production as it is very unstable and was built as a learning opportunity, not a spec'd HTTP server. This was not built using AI, just straight human-slop. The name flakey was chosen because of how delicate and bug ridden it is lol.

## **Goals**
The goal of this project was originally used to learn network programming, I originally started with a hand-rolled JSON parser and figured it would fit well into a micro http framework. Flakey is very limited in what it can do, and doesn't have any modern features used in enterprise solutions.

Flakey is only able to serve static HTML content, read/write JSON and handle URL routing. It lacks authentication, logging, encryption modules, and other goodies that are usually shipped with HTTP frameworks.

## **Features** (Current and planned)
- Lightweight HTTP server
- Easy setup and usage
- Serve static HTML
- Create your own routes, and route handlers - WIP
- Support for reading and writing JSON - WIP

## **Usage**
### Starting Server 
```
#include "server.hpp"
#include "request.hpp"
#include "response.hpp"

int main(){
  // Create the server object
  int port    { 8080 };
  int backlog { 10 };
  HTTPServer::Server server(port, backlog);

  // Run the server
  server.run();
}
```
### Building routes
- User is responsible for building the handlers
- All handlers must have a return type of HTTPResponse
- All handlers must have a HTTPRequest& param

**Adding a route**
```
// addRoute(string METHOD, string PATH, callable)
server.addRoute("GET", "/index", index_handler)
```

**Route Handler**
```
HTTPResponse exampleHandler(HTTPRequest& req)
{
  std::string header = "HTTP/1.1";
  std::string payload = "<h1> Hello, World! </h1>";
  short status_code = 200;
  std::string content_type = "text/html";

  HTTPResponse res(header, payload, content_type, status_code);
  return res;
}
```

### Rendering HTML
- In order to render HTML, you only need to read a .html file into a string, then pass that string as the "payload"

## ToDos:
- JSON Parser 
- .html, .css, and .js file handling
- Logging
- Stop hard coding shit
- Serializer is garbage, keep working on it

## Resoucres used:
- Beej network programming guide: https://beej.us/guide/bgnet/html/split/index.html
- POSIX Programmer's Manual (man pages)
- JSON parsing inspo: https://dev.to/uponthesky/c-making-a-simple-json-parser-from-scratch-250g
- cppreference: https://cppreference.com/
