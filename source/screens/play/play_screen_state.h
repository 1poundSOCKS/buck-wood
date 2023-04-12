#ifndef _play_screen_state_
#define _play_screen_state_

#include "framework.h"
#include "game_objects.h"
#include "level_state.h"
#include "timers.h"
#include "sound_data.h"

struct play_screen_state
{
  play_screen_state(
    game_level_data_index::const_iterator currentLevelDataIterator, 
    game_level_data_index::const_iterator endLevelDataIterator,
    const screen_render_data& renderData,
    const sound_data& bespokeSoundData
  );

  auto LoadLevel(const game_level_data& levelData) -> void;
  auto AddPlayer(float x, float y) -> void;
  [[nodiscard]] auto CreateViewTransform(const D2D1_SIZE_F& renderTargetSize, float renderScale = 1.0) const -> D2D1::Matrix3x2F;
  [[nodiscard]] auto PlayerIsDead() -> bool;
  auto PlaySoundEffects(const global_sound_buffer_selector& soundBuffers) const -> void;
  [[nodiscard]] auto GetMouseDiagnostics() const -> std::wstring;

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

  std::unique_ptr<level_state> levelState;
  std::vector<float> levelTimes;

  player_ship player;
  std::shared_ptr<player_control_state> m_controlState;

  float mouseX = 0;
  float mouseY = 0;

  bool playerShot = false;
  bool targetShot = false;
};

#endif
