#ifndef _play_screen_
#define _play_screen_

#include "framework.h"
#include "object_container_view.h"
#include "stopwatch.h"
#include "sound_data.h"
#include "screen_input_state.h"
#include "level_control_state.h"
#include "player_ship.h"
#include "bullet.h"
#include "level_target.h"
#include "level_island.h"
#include "sound_data.h"
#include "level_timer.h"
#include "level_state.h"
#include "mouse_cursor.h"
#include "diagnostics.h"
#include "game_level_data_loader.h"

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

  auto UpdateLevelState(const screen_input_state& inputState, int64_t elapsedTicks) -> void;
  
  [[nodiscard]] auto PausePressed(const screen_input_state& inputState) -> bool;
  [[nodiscard]] auto QuitPressed(const screen_input_state& inputState) -> bool;

  [[nodiscard]] auto LoadFirstLevel() -> bool;
  [[nodiscard]] auto LoadNextLevel() -> bool;
  auto LoadCurrentLevel() -> void;

  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<IDWriteFactory> m_dwriteFactory;

  bool m_paused = false;
  bool m_gameComplete = false;
  bool m_continueRunning = true;

  active_object_container m_levelContainer;
  object_container_view m_levelView;

  passive_object_container m_overlayContainer;
  object_container_view m_overlayView;

  std::vector<float> levelTimes;

  game_level_data_loader m_gameLevelDataLoader;

  level_control_data m_levelControlData;

  bool m_playerShot = false;
  bool m_targetActivated = false;
};

#endif
