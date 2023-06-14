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
#include "level_map.h"
#include "text_box.h"

class play_screen
{
public:

  play_screen();
  auto Refresh(const screen_input_state& inputState, int64_t ticks) -> bool;

private:

  enum class stage { pre_play, playing, post_play };

  auto Update(const screen_input_state& inputState, int64_t frameInterval, diagnostics_data_collection& diagnosticsData) -> level_container::update_events_ptr;
  auto Render(diagnostics_data_collection& diagnosticsData) const -> void;
  auto PostPresent(const level_container::update_events_ptr& levelUpdateEvents) const -> void;
  auto FormatDiagnostics(diagnostics_data_inserter_type diagnosticsDataInserter) const -> void;

  auto PrePlay(const screen_input_state& inputState, int64_t frameInterval) -> void;
  auto Playing(const screen_input_state& inputState, int64_t frameInterval) -> level_container::update_events_ptr;
  auto PostPlay(const screen_input_state& inputState, int64_t frameInterval) -> void;

  auto UpdateLevel(const screen_input_state& inputState, int64_t elapsedTicks) -> level_container::update_events_ptr;
  auto GetLevelRenderTransform() const -> screen_transform;
  auto GetOverlayRenderTransform() const -> screen_transform;
  auto GetCameraPosition(D2D1_SIZE_F renderTargetSize) const -> camera_sequence::camera_position;

  [[nodiscard]] auto PausePressed(const screen_input_state& inputState) -> bool;
  [[nodiscard]] auto LoadFirstLevel() -> bool;
  [[nodiscard]] auto LoadNextLevel() -> bool;
  [[nodiscard]] auto GetMenuDef() -> menu_def;

  performance::frame_data m_frameData;
  int64_t m_frameTicks = 0;
  stage m_stage = stage::pre_play;
  bool m_paused = false;
  bool m_continueRunning = true;
  int64_t m_stageTicks = 0;
  std::unique_ptr<level_container> m_levelContainer;
  mouse_cursor m_cursor;
  menu m_menu;
  game_level_data_loader m_gameLevelDataLoader;
  level_map m_levelMap;
  std::vector<int64_t> m_levelTimes;
  camera_sequence m_startSequence;
  camera_sequence m_endSequence;

  static inline float m_playZoom = 0.6f;
};
