#ifndef _global_state_
#define _global_state_

#include "game_level_data.h"

enum screen_id { screen_none, screen_main_menu, screen_play, screen_level_editor };

class global_state
{
public:

  static auto load(const std::wstring& dataPath) -> void;
  static auto save() -> void;

private:

  static global_state* m_globalState;

  global_state(const std::wstring& dataPath);

  std::wstring dataPath;
};

#endif
