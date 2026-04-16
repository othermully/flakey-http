#include "request.hpp"
#include <sstream>
#include <string>

namespace HTTPParser{
using Request = HTTPRequest;

class Parser{
public:

  Request parseRequest(const std::string& raw_req)
  {
    Request req;
    std::istringstream f(raw_req);
    std::string line;

    size_t status_pos = raw_req.find("\n");
    std::string status_line = raw_req.substr(0, status_pos);

    req.m_path   = getPath(status_line);
    req.m_method = getMethod(status_line);

    while (std::getline(f, line))
    {
      char delimiter = ':';
      size_t pos = line.find(delimiter);
      std::string value = line.substr(0, pos);
      pos += 2; // Skip the delim and whitespace -> Needs upgraded

      if (value == "Host")            req.m_host            = line.substr(pos);
      if (value == "User-Agent")      req.m_user_agent      = line.substr(pos);
      if (value == "Accept")          req.m_accept          = line.substr(pos);
      if (value == "Accept-Language") req.m_accept_language = line.substr(pos);
      if (value == "Accept-Encoding") req.m_accept_encoding = line.substr(pos);
      if (value == "Referer")         req.m_referer         = line.substr(pos);
      if (value == "Content-Type")    req.m_content_type    = line.substr(pos);
      if (value == "Connection")      req.m_connection      = line.substr(pos);
      if (value == "Sec-Fetch-Dest")  req.m_sec_fetch_dest  = line.substr(pos);
      if (value == "Sec-Fetch-Mode")  req.m_sec_fetch_mode  = line.substr(pos);
      if (value == "Sec-Fetch-Site")  req.m_sec_fetch_site  = line.substr(pos);
      if (value == "Sec-Fetch-User")  req.m_sec_fetch_user  = line.substr(pos);
      if (value == "Priority")        req.m_priority        = line.substr(pos);
    }

    return req;
  }

private:

  std::string getMethod(std::string& status_line)
  {
    size_t pos = status_line.find(" ");
    std::string method = status_line.substr(0, pos);
    return method;
  }

  std::string getPath(std::string& status_line)
  {
    size_t pos = status_line.find("/");
    std::string next_space = status_line.substr(pos);
    pos = next_space.find(" ");
    std::string path = next_space.substr(0, pos);
    return path;
  }

};

} // End of namespace
