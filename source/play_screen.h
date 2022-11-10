#ifndef _play_screen_
#define _play_screen_

#define UNICODE

#include <memory>
#include <list>

#include "framework/framework.h"
#include "control_state.h"
#include "sound_buffers.h"
#include "game_objects.h"
#include "diagnostics.h"
#include "global_state.h"

struct play_screen_control_state
{
  bool returnToMenu = false;
  bool restartPlay = false;
  bool pausePlay = false;
  bool thrust = false, shoot = false;
  render_target_mouse_data renderTargetMouseData;
};

struct play_screen_state
{
  play_screen_state(const global_state& globalState, const system_timer& timer);

  const global_state& globalState;
  const system_timer& systemTimer;

  d2d_brushes brushes;
  dwrite_text_formats textFormats;

  bool returnToMenu = false;
  
  enum STATE { state_playing, state_paused, state_level_complete, state_game_complete, state_player_dead };
  STATE state = state_playing;

  mouse_cursor mouseCursor;
  float renderTargetMouseX = 0, renderTargetMouseY = 0;
  
  std::vector<std::unique_ptr<game_level_data>>::const_iterator currentLevelDataIterator;
  std::unique_ptr<game_level> currentLevel;

  std::unique_ptr<player_ship> player;
  std::list<std::unique_ptr<bullet>> bullets;

  std::unique_ptr<stopwatch> levelTimer;
  std::unique_ptr<stopwatch> pauseTimer;
  std::unique_ptr<stopwatch> shotTimer;

  float levelMouseX = 0, levelMouseY = 0;

  bool playerShot = false, targetShot = false;

  std::vector<float> levelTimes;

  D2D1::Matrix3x2F viewTransform;

  static const int shotTimeNumerator = 1;
  static const int shotTimeDenominator = 60;
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
