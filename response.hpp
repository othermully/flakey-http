#pragma once
#include <iostream>

class HTTPResponse{
public:
  std::string m_header{};
  std::string m_payload{};
  std::string m_content_type{};
  short m_status_code{};

  HTTPResponse(std::string& header, std::string& payload,
      std::string& content_type, short status_code)
  {
    m_header       = header;
    m_payload      = payload;
    m_content_type = content_type;
    m_status_code  = status_code;
  }
};
