#ifndef _game_level_file_
#define _game_level_file_

#include <iostream>
#include <fstream>

#ifdef USE_JSON
#include <json.h>
#endif

struct game_level_file
{
  game_level_file();

  std::string name;;
};

#endif
