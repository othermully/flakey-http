#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <filesystem>

namespace HTMLRender{

class Renderer{
public:

  std::string renderHtml(const std::string& file_name)
  {

    std::string path = "./templates/" + file_name;

    if (std::filesystem::exists(path)) {
      std::cout << "File found: " << path << '\n';

      std::ifstream ifs(file_name);
      std::string html( (std::istreambuf_iterator<char>(ifs) ),( std::istreambuf_iterator<char>() ) );

      return html;
    }

    return "";
  }
};

} // end of namespace
