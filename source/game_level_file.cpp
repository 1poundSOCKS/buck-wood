#include "game_level_file.h"
#include <iostream>

game_level_file::game_level_file()
{
  std::ifstream fileStream("data/test_level.json", std::ifstream::binary);
  Json::Reader reader;
  Json::Value obj;
  reader.parse(fileStream, obj);
  Json::String nameJson = obj["name"].asString();
  name = nameJson;
  std::cout << nameJson << std::endl;
}
