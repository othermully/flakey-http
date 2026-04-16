#pragma once
#include <iostream>

class HTTPRequest{
public:
  std::string m_method{};
  std::string m_path{};
  std::string m_body{}; // Not sure how to handle this one yet

  std::string m_referer{};
  std::string m_content_length{};
  std::string m_host{};
  std::string m_user_agent{};
  std::string m_accept{};
  std::string m_accept_language{};
  std::string m_accept_encoding{};
  std::string m_content_type{};
  std::string m_origin{};
  std::string m_connection{};
  std::string m_sec_fetch_dest{};
  std::string m_sec_fetch_mode{};
  std::string m_sec_fetch_site{};
  std::string m_sec_fetch_user{};
  std::string m_priority{};

};
