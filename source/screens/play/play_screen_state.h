#ifndef _play_screen_state_
#define _play_screen_state_

#include "framework.h"
#include "game_objects.h"
#include "level_state.h"
#include "timers.h"
#include "sound_data.h"
#include "player_ship.h"
#include "bullet.h"
#include "screen_input_state.h"
#include "screen_render_data.h"
#include "level_control_state.h"
#include "area_grid.h"
#include "explosion.h"
#include "level_target.h"
#include "level_island.h"
#include "sound_data.h"
#include "game_level_data.h"
#include "level_timer.h"

struct play_screen_state
{
  play_screen_state(
    game_level_data_index::const_iterator currentLevelDataIterator, 
    game_level_data_index::const_iterator endLevelDataIterator,
    const screen_render_data& renderData,
    const sound_data& bespokeSoundData
  );

  auto LoadLevel(const game_level_data& levelData) -> void;
  auto UpdateLevelState(const screen_input_state& inputState) -> void;
  auto AddPlayer(float x, float y) -> void;
  [[nodiscard]] auto CreateViewTransform(const D2D1_SIZE_F& renderTargetSize, float renderScale = 1.0) -> D2D1::Matrix3x2F;
  auto PlaySoundEffects(const global_sound_buffer_selector& soundBuffers) const -> void;
  [[nodiscard]] auto GetMouseDiagnostics() const -> std::wstring;
  
  void OnGamePaused(const screen_input_state& inputState);
  void OnGameRunning(const screen_input_state& inputState);
  void OnGamePlaying(const screen_input_state& inputState);
  bool AllLevelsAreComplete();
  void LoadNextLevel();
  bool ScreenTransitionTimeHasExpired();
  void SetScreenTransitionDelay(int timeInSeconds);

  const screen_render_data renderData;
  const sound_data soundData;

  performance_counter::data timer = { 0, 0, 0 };
  
  int64_t pauseStart = 0;
  int64_t pauseTotal = 0;
  int64_t levelStart = 0;
  int64_t transitionEnd = 0;

  enum mode_type { playing, paused, level_complete, game_complete, player_dead };
  mode_type mode = playing;
  bool continueRunning = true;

  game_level_data_index::const_iterator currentLevelDataIterator;
  game_level_data_index::const_iterator endLevelDataIterator;

  render_target_mouse_data renderTargetMouseData;
  game_rect m_viewRect;
  D2D1::Matrix3x2F m_viewTransform;

  std::unique_ptr<level_state> levelState;
  int64_t levelTimeLimit;
  std::vector<float> levelTimes;

  std::unique_ptr<player_ship> player;
  std::unique_ptr<level_timer> m_levelTimer;

  float mouseX = 0;
  float mouseY = 0;

  bool playerShot = false;
  bool targetShot = false;
};

#endif
