#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <filesystem>

// TODO: Probably don't even need this class yet, handle this within the router

namespace HTMLRender{

class Renderer{
public:

  std::string renderHtml(const std::string& file_name)
  {

    std::string path = "./templates/" + file_name;

    if (std::filesystem::exists(path)) {
      std::ifstream ifs(path);
      std::string html ( (std::istreambuf_iterator<char>(ifs) ),( std::istreambuf_iterator<char>() ) );

      return html;
    }

    return "";
  }

};

} // end of namespace
