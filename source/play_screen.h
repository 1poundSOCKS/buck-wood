#pragma once

#include "framework.h"
#include "passive_object_container.h"
#include "screen_transform.h"
#include "sound_data.h"
#include "screen_input_state.h"
#include "sound_data.h"
#include "level_state.h"
#include "mouse_cursor.h"
#include "diagnostics.h"
#include "game_level_data_loader.h"
#include "menu_def.h"
#include "camera_sequence.h"
#include "play_camera_transform.h"

class play_screen
{
public:

  play_screen();

  auto Update(const screen_input_state& inputState, int64_t frameInterval) -> void;
  auto Render() const -> void;
  auto PostPresent() const -> void;
  [[nodiscard]] auto ContinueRunning() const -> bool;
  auto FormatDiagnostics(diagnostics_data_inserter_type diagnosticsDataInserter) const -> void;

private:

  enum class stage { pre_play, playing, post_play };

  auto Initialize() -> void;

  auto PrePlay(const screen_input_state& inputState, int64_t frameInterval) -> void;
  auto Playing(const screen_input_state& inputState, int64_t frameInterval) -> void;
  auto PostPlay(const screen_input_state& inputState, int64_t frameInterval) -> void;

  auto UpdateLevel(const screen_input_state& inputState, int64_t elapsedTicks) -> void;
  auto GetLevelRenderTransform() const -> screen_transform;
  auto GetOverlayRenderTransform() const -> screen_transform;
  auto GetCameraPosition(D2D1_SIZE_F renderTargetSize) const -> camera_sequence::camera_position;

  [[nodiscard]] auto PausePressed(const screen_input_state& inputState) -> bool;
  [[nodiscard]] auto LoadFirstLevel() -> bool;
  [[nodiscard]] auto LoadNextLevel() -> bool;
  auto LoadCurrentLevel() -> void;
  [[nodiscard]] auto GetMenuDef() -> menu_def;

  int64_t m_frameTicks = 0;
  stage m_stage = stage::pre_play;
  bool m_paused = false;
  bool m_continueRunning = true;
  int64_t m_stageTicks = 0;
  std::unique_ptr<level_container> m_levelContainer;
  passive_object_container m_overlayContainer;
  game_level_data_loader m_gameLevelDataLoader;
  std::vector<int64_t> m_levelTimes;
  camera_sequence m_startSequence;
  camera_sequence m_endSequence;

  static inline float m_playZoom = 0.6f;
};
