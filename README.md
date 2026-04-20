# Flakey HTTP

> **Warning**  
> This project is **not RFC-compliant** and is **not suitable for production use**.  
> It is an experimental, educational implementation of a minimal HTTP-like server written in C++.  
> If you need a reliable HTTP server or framework, use a mature library such as:  
> https://github.com/yhirose/cpp-httplib

---

## Overview

**Flakey HTTP** is a minimal HTTP server implementation written in C++ using low-level socket programming (POSIX APIs). It provides a basic framework for:

- Accepting TCP connections
- Parsing extremely limited HTTP-like requests
- Dispatching requests via a simple routing mechanism
- Constructing and returning HTTP-like responses
- Basic JSON read/write experimentation (incomplete)

This project intentionally avoids external dependencies and frameworks. It is designed to explore how HTTP servers *might* work internally, not to correctly or fully implement the HTTP specification.

---

## Design Goals

The primary purpose of this project is educational:

- Learn low-level network programming (sockets, `bind`, `listen`, `accept`)
- Explore request/response lifecycles in HTTP servers
- Experiment with basic routing and handler abstractions
- Implement a simple (non-compliant) JSON parser and serializer

Non-goals include:

- Standards compliance (HTTP/1.1, HTTP/2, etc.)
- Performance optimization
- Security
- Robustness or fault tolerance

---

## What This Project Implements

- Single-process, synchronous TCP server
- Basic request parsing (method + path only, partial headers at best)
- Route registration using `(METHOD, PATH)` mapping
- User-defined request handlers
- Manual response construction
- Static file serving via string loading
- Experimental JSON parsing/serialization (incomplete and non-compliant)

---

## What This Project Does NOT Implement

This project omits most features required for a real HTTP server, including:

### Protocol Support
- Full HTTP/1.1 compliance
- Header parsing/validation
- Chunked transfer encoding
- Persistent connections (keep-alive)
- Proper status line and header formatting guarantees

### Networking & Concurrency
- No multithreading or async I/O
- No connection pooling
- No scalability considerations

### Security
- No TLS/SSL (HTTPS)
- No authentication or authorization
- No input validation or sanitization
- Vulnerable to malformed requests

### Server Features
- No logging
- No middleware system
- No configuration system
- No MIME type detection beyond manual input

### JSON Handling
- Partial, non-compliant parser
- No full spec coverage (e.g., edge cases, encoding, strict validation)
- Serializer is incomplete and unreliable

---

## Usage

### Starting the Server

```cpp
#include "server.hpp"
#include "request.hpp"
#include "response.hpp"

int main() {
    int port    = 8080;
    int backlog = 10;

    HTTPServer::Server server(port, backlog);
    server.run();
}
```

#### Notes:
- Uses blocking POSIX socket APIs (`socket`, `bind`, `listen`, `accept`)
- No signal handling or graceful shutdown mechanism
- No runtime config (everything is hardcoded at instantiation

### Routing:
Routing is implemented as a simple mapping of `(METHOD, PATH)` -> handler.

```cpp
server.addRoute("GET", "/index", index_handler);
```

#### Characteristics:
- Method and path are treated as raw strings
- No normalization (e.g. `/index` vs `/index/`
- No wildcard or parameterized routes
- duplicate routes are not checked or prevented

### Handler Interface
Handlers must conform to the following signature:
```cpp
HTTPResponse handler(HTTPRequest& request);
```

- `HTTPRequest` is passed by reference
- `HTTPResponse` must be fully constructed and returned
- No middleware, chaining, or pre/post-processing hooks exist

#### Example Handler:
```cpp
HTTPResponse exampleHandler(HTTPRequest& req)
{
    std::string protocol     = "HTTP/1.1";
    std::string payload      = "<h1>Hello, World!</h1>";
    short status_code        = 200;
    std::string content_type = "text/html";

    return HTTPResponse(protocol, payload, content_type, status_code);
}
```

#### Notes:
- No enforcement of HTTP formatting rules
- Status codes are not validated
- Response correctness is entirely user-defined
- Render HTML via the payload using the `renderHTML` member function in the `Renderer` class

### Request Object
`HTTPRequest` exposes a minimal interface. Depending on implementation state, it may include:
- `method` (string)
- `path` (string)
- `body` (string, optional)
- partial or raw headers 


