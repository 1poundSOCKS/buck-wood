#ifndef _global_state_
#define _global_state_

#include "framework.h"
#include "dwrite_text_formats.h"
#include "sound_buffers.h"
#include "game_objects.h"
#include "render.h"
#include "diagnostics.h"

enum screen_id { screen_none, screen_main_menu, screen_play, screen_level_editor };

struct global_state
{
  global_state(const d2d_app& app, const std::wstring& dataPath);

  std::wstring dataPath;
  render_brushes renderBrushes;
  dwrite_text_formats textFormats;
  sound_buffers soundBuffers;
  std::unique_ptr<game_level_data_index> gameLevelDataIndex;

  screen_id currentScreenId = screen_main_menu;

  bool saveGameLevelData = false;
  bool gameLevelDataIndexUpdated = false;
};

void FormatDiagnostics(std::back_insert_iterator<diagnostics_data> diagnosticsData, const global_state& globalState);

#endif
