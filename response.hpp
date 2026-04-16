#pragma once
#include <iostream>

class HTTPResponse{
public:
  std::string m_header{};
  std::string m_payload{};

  HTTPResponse(std::string& header, std::string& payload){
    m_header  = header;
    m_payload = payload;
  }
};
