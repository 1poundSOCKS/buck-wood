#ifndef _global_state_
#define _global_state_

#include "framework.h"
#include "dwrite_text_formats.h"
#include "sound_buffers.h"
#include "game_objects.h"
#include "render.h"

struct global_state
{
  global_state(const d2d_app& app, const std::wstring& dataPath);

  std::wstring dataPath;
  // d2d_brushes brushes;
  render_brushes renderBrushes;
  dwrite_text_formats textFormats;
  sound_buffers soundBuffers;
  std::unique_ptr<game_level_data_index> gameLevelDataIndex;

  bool saveGameLevelData = false;
  bool gameLevelDataIndexUpdated = false;
};

#endif
