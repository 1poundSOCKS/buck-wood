#ifndef _global_state_
#define _global_state_

#include "framework.h"
#include "game_objects.h"
#include "render.h"
#include "diagnostics.h"

enum screen_id { screen_none, screen_main_menu, screen_play, screen_level_editor };

struct global_state
{
  global_state(const std::wstring& dataPath);

  std::wstring dataPath;
  std::unique_ptr<game_level_data_index> gameLevelDataIndex;

  screen_id currentScreenId = screen_main_menu;

  bool saveGameLevelData = false;
  bool gameLevelDataIndexUpdated = false;
};

#endif
