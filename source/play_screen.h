#ifndef _play_screen_
#define _play_screen_

#include "framework.h"
#include "game_objects.h"
#include "level_object_container.h"
#include "object_container_view.h"
#include "timers.h"
#include "sound_data.h"
#include "player_ship.h"
#include "bullet.h"
#include "screen_input_state.h"
#include "level_control_state.h"
#include "area_grid.h"
#include "level_target.h"
#include "level_island.h"
#include "sound_data.h"
#include "game_level_data.h"
#include "level_timer.h"
#include "level_state.h"
#include "mouse_cursor.h"
#include "diagnostics.h"
#include "game_level_data_loader.h"
#include "stopwatch.h"

class play_screen
{
public:

  play_screen();

  auto Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void;
  auto Update(const screen_input_state& inputState) -> void;
  auto Render() const -> void;
  auto PlaySoundEffects() const -> void;
  [[nodiscard]] auto ContinueRunning() const -> bool;
  auto FormatDiagnostics(diagnostics_data_inserter_type diagnosticsDataInserter) const -> void;

private:

  static level_control_state GetLevelControlState(const screen_input_state& inputState);

  auto UpdateMouseCursorPosition() -> void;
  auto UpdateLevelState(const screen_input_state& inputState, int64_t elapsedTicks) -> void;
  auto PlaySoundEffects(const global_sound_buffer_selector& soundBuffers) const -> void;
  [[nodiscard]] auto GetMouseDiagnostics() const -> std::wstring;
  
  [[nodiscard]] auto PausePressed(const screen_input_state& inputState) -> bool;
  [[nodiscard]] auto QuitPressed(const screen_input_state& inputState) -> bool;

  [[nodiscard]] auto LoadFirstLevel() -> bool;
  [[nodiscard]] auto LoadNextLevel() -> bool;
  auto LoadCurrentLevel() -> void;

  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<IDWriteFactory> m_dwriteFactory;

  stopwatch m_levelStopwatch;

  bool m_paused = false;
  bool m_gameComplete = false;
  bool m_continueRunning = true;

  render_target_mouse_data renderTargetMouseData;
  game_rect m_viewRect;
  D2D1::Matrix3x2F m_viewTransform;

  object_container_view m_containerView;
  level_object_container m_objectContainer;

  int64_t levelTimeLimit;
  std::vector<float> levelTimes;

  game_level_data_loader m_gameLevelDataLoader;

  mouse_cursor m_mouseCursor;

  player_ship::control_data m_playerControlData;
  level_timer::control_data m_timerControlData;
  level_state::control_data m_stateControlData;

  D2D1_POINT_2F m_mouseLevelPosition = { 0, 0 };

  bool m_playerShot = false;
  bool m_targetActivated = false;
};

#endif
