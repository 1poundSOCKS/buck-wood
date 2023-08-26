#pragma once

#include "framework.h"
#include "screen_transform.h"
#include "sound_data.h"
#include "screen_input_state.h"
#include "sound_data.h"
#include "mouse_cursor.h"
#include "diagnostics.h"
#include "game_level_data_loader.h"
#include "menu_def.h"
#include "camera_sequence.h"
#include "play_camera_transform.h"
#include "text_box.h"
#include "level_radar.h"
#include "player_shields.h"
#include "gamepad_thumbstick.h"

class play_screen
{
public:

  play_screen();
  auto Refresh(int64_t ticks) -> bool;

private:

  enum class stage { pre_play, playing, post_play };

  auto Update(int64_t frameInterval) -> level_container::update_events_ptr;
  auto Render() const -> void;
  auto PostPresent(const level_container::update_events_ptr& levelUpdateEvents) const -> void;

  auto PrePlay(int64_t frameInterval) -> void;
  auto Playing(int64_t frameInterval) -> level_container::update_events_ptr;
  auto PostPlay(int64_t frameInterval) -> void;

  [[nodiscard]] auto GetLevelInput(const screen_input_state& input, const screen_transform& transform) const -> level_input;
  [[nodiscard]] static auto GetThumbStickAngle(short lx, short ly, float rotation = 0) -> std::optional<float>;
  auto UpdateLevel(int64_t elapsedTicks) -> level_container::update_events_ptr;
  auto GetLevelRenderTransform() const -> screen_transform;
  auto GetOverlayRenderTransform() const -> screen_transform;
  auto GetCameraPosition(D2D1_SIZE_F renderTargetSize) const -> camera_sequence::camera_position;

  [[nodiscard]] auto PausePressed() -> bool;
  [[nodiscard]] auto LoadFirstLevel() -> bool;
  [[nodiscard]] auto LoadNextLevel() -> bool;
  [[nodiscard]] auto GetMenuDef() -> menu_def;

  stage m_stage = stage::pre_play;
  bool m_paused = false;
  bool m_continueRunning = true;
  int64_t m_stageTicks = 0;
  
  std::unique_ptr<level_container> m_levelContainer;
  
  mouse_cursor m_cursor;
  menu m_menu;
  level_radar m_levelRadar;
  player_shields m_playerShields;

  game_level_data_loader m_gameLevelDataLoader;
  
  std::vector<int64_t> m_levelTimes;
  camera_sequence m_startSequence;
  camera_sequence m_endSequence;

  static inline float m_playZoom = 0.6f;
};

[[nodiscard]] inline auto play_screen::GetThumbStickAngle(short x, short y, float rotation) -> std::optional<float>
{
  gamepad_thumbstick thumbstick { x, y, 5000 };

  return ( thumbstick.Distance() < 5000 ) ? 
    std::nullopt : std::optional<float> { thumbstick.Angle() + rotation };
}
