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

## **Requirements**
## **Usage**
