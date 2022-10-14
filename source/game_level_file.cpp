#include "game_level_file.h"

game_level_file::game_level_file()
{
#ifdef USE_JSON
  std::ifstream fileStream("data/test_level.json", std::ifstream::binary);
  Json::Reader reader;
  Json::Value obj;
  reader.parse(fileStream, obj);
  Json::String nameJson = obj["name"].asString();
  name = nameJson;
#endif
}
