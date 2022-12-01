#ifndef _global_state_
#define _global_state_

#include "framework.h"
#include "sound_buffers.h"
#include "game_objects.h"
#include "render.h"
#include "diagnostics.h"

enum screen_id { screen_none, screen_main_menu, screen_play, screen_level_editor };

struct global_state
{
  global_state(const std::wstring& dataPath, ID2D1RenderTarget* renderTarget, IDirectSound8* directSound);

  std::wstring dataPath;
  sound_buffers soundBuffers;
  std::unique_ptr<game_level_data_index> gameLevelDataIndex;

  screen_id currentScreenId = screen_main_menu;

  bool saveGameLevelData = false;
  bool gameLevelDataIndexUpdated = false;
};

void FormatDiagnostics(std::back_insert_iterator<diagnostics_data> diagnosticsData, const global_state& globalState);

#endif
