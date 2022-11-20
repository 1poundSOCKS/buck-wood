#ifndef _play_screen_
#define _play_screen_

#include "framework.h"
#include "control_state.h"
#include "sound_buffers.h"
#include "game_objects.h"
#include "diagnostics.h"
#include "global_state.h"
#include "level_state.h"

struct play_screen_control_state
{
  bool returnToMenu = false;
  bool restartPlay = false;
  bool pausePlay = false;
  level_control_state levelControlState;
};

struct play_screen_state
{
  play_screen_state(const global_state& globalState, const system_timer& timer);

  const global_state& globalState;
  const system_timer& systemTimer;

  d2d_brushes brushes;
  dwrite_text_formats textFormats;
  render_brushes renderBrushes;

  bool returnToMenu = false;
  
  enum STATE { state_playing, state_paused, state_level_complete, state_game_complete, state_player_dead };
  STATE state = state_playing;

  std::vector<std::unique_ptr<game_level_data>>::const_iterator currentLevelDataIterator;

  std::unique_ptr<stopwatch> levelTimer;
  std::unique_ptr<stopwatch> pauseTimer;

  std::vector<float> levelTimes;

  D2D1::Matrix3x2F viewTransform;

  static const int shotTimeNumerator = 1;
  static const int shotTimeDenominator = 60;

  std::unique_ptr<level_state> levelState;
};

struct play_screen_sounds
{
  play_screen_sounds(const global_state& globalAssets);

  sound_buffer_player thrust;
  sound_buffer_player shoot;
  sound_buffer_player targetActivated;
};

void RefreshControlState(play_screen_control_state& controlState, const control_state& baseControlState);
void UpdateScreenState(play_screen_state& screenState, const play_screen_control_state& controlState, const system_timer& timer);
void RenderFrame(const d2d_frame& frame, const play_screen_state& screenState);
void UpdateSound(const play_screen_state& screenState, const play_screen_sounds& soundBuffers);
void FormatDiagnostics(diagnostics_data& diagnosticsData, const play_screen_state& screenState, const play_screen_control_state& controlState);

#endif
