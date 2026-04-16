#pragma once
#include <cctype>
#include <cstddef>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <vector>
#include <variant>

namespace JSONParser{
class Parser{
public:

  struct JSON;
  using Object = std::map<std::string, JSON>;
  using Array  = std::vector<JSON>;

  JSON parseObject(const std::string& str, size_t& i);
  JSON parseString(const std::string& str, size_t& i);
  JSON parseArray(const std::string& str, size_t& i);

  struct JSON{
    std::variant<
      std::nullptr_t,
      double,
      std::string,
      Object,
      Array
      > value;
  };

  void skipWhitespace(const std::string& str, size_t& i){
    while (i < str.size() && std::isspace(str[i])) {
      ++i;
    }
  }

  std::string_view getSubstring(std::string_view sv, size_t& i){
    return sv.substr(i, sv.size());
  }

  // Dispatcher
  JSON parseValue(const std::string& str, size_t& i){
    skipWhitespace(str, i);

    if (str[i] == '"') {
      return parseString(str, i);
    }
    if (str[i] == '{') {
      return parseObject(str, i);
    }
    if (str[i] == '[') {
      return parseArray(str, i);
    }
    if (std::isdigit(str[i])) {
      return parseNumber(str, i);
    }

    return JSON{}; // This should be handled, return empty struct for now
  }

  JSON parseNumber(const std::string& str, size_t& i){
    skipWhitespace(str, i);
    double d{};
    std::string temp{};
    while (std::isdigit(str[i])) {
      temp += static_cast<char>(str[i]);
      ++i;
    }

    d = std::stoi(temp);
    return JSON{d};
  }


private:


};

} // End of namespace
