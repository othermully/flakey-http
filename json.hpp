#pragma once
#include <cctype>
#include <cstddef>
#include <iostream>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
#include <variant>

// TODO: Need to build an API to allow users to easily access JSON key/values

namespace JSONParser{
class Parser{
public:

  struct JSON;
  using Object = std::map<std::string, JSON>;
  using Array  = std::vector<JSON>;

  struct JSON{
    std::variant<
      std::nullptr_t,
      double,
      std::string,
      Object,
      Array
      > value;
  };

  // Dispatcher
  JSON parseValue(const std::string& str, size_t& i)
  {
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



  
private:
  void skipWhitespace(const std::string& str, size_t& i)
  {
    while (i < str.size() && std::isspace(str[i])) {
      ++i;
    }
  }

  std::string_view getSubstring(std::string_view sv, size_t& i)
  {
    return sv.substr(i, sv.size());
  }

  JSON parseNumber(const std::string& str, size_t& i)
  {
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

  JSON parseObject(const std::string& str, size_t& i)
  {
    skipWhitespace(str, i);
    Object obj{};

    ++i; // Consume the opening '{' - Expect a key string
    while (true) {
      std::string key = std::get<std::string>(parseString(str, i).value);
      if (str[i] != ':') {
        throw std::runtime_error("Expected ':'");
      }

      ++i; // Consume the ':'
      skipWhitespace(str, i);

      // expect value here
      JSON value = parseValue(str, i);
      skipWhitespace(str, i);

      obj[key] = value;

      if (str[i] == ',') {
        skipWhitespace(str, i);
        ++i; // consume ','
        continue;
      }

      if (str[i] == '}') {
        ++i; // consume the closing '}'
        return JSON{obj};
      }
      else {
        throw std::runtime_error("Didn't receive correct char");
      }
    }
  }

  JSON parseArray(const std::string& str, size_t& i)
  {
    Array arr;
    skipWhitespace(str, i);
    ++i; // consume the opening '['

    while (true) {
      skipWhitespace(str, i);
      JSON value = parseValue(str, i);
      arr.push_back(value);

      if (str[i] == ',') {
        skipWhitespace(str, i);
        ++i;
        continue;
      }

      if (str[i] == ']') {
        ++i; // consume closing ']'
        return JSON{arr};
      }
    }
  }

  JSON parseString(const std::string& str, size_t& i)
  {
    skipWhitespace(str, i);
    std::string s{};
    ++i; // Consume the opening '"'
    if (str[i] == '"') {
      ++i; // consume closing '"'
      return JSON{s};
    }

  }
};

class API{
// TODO: Builder JSON key/value access functions here
// TODO: I think this should also serialize parsed JSON in order to send back to client
// Access JSON objects like this: JSON j = jparser.parseValue(string);
// std::string name = j["name"];
// find type -> get value -> access fields/array elements
public:
  std::optional<std::string> getValue(Parser::JSON& t, std::string_view input_key);

private:
  void getType();

};

} // End of namespace
