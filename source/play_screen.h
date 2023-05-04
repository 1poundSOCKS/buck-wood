#pragma once

#include "framework.h"
#include "passive_object_container.h"
#include "render_target_view.h"
#include "sound_data.h"
#include "screen_input_state.h"
#include "sound_data.h"
#include "level_state.h"
#include "mouse_cursor.h"
#include "diagnostics.h"
#include "game_level_data_loader.h"
#include "menu_def.h"

class play_screen
{
public:

  play_screen();

  auto Initialize(ID2D1RenderTarget* renderTarget) -> void;
  auto Update(const screen_input_state& inputState) -> void;
  auto Render() const -> void;
  auto PlaySoundEffects() const -> void;
  [[nodiscard]] auto ContinueRunning() const -> bool;
  auto FormatDiagnostics(diagnostics_data_inserter_type diagnosticsDataInserter) const -> void;

private:

  enum view_type { view_starting, view_playing, view_ending };

  [[nodiscard]] auto PausePressed(const screen_input_state& inputState) -> bool;

  [[nodiscard]] auto LoadFirstLevel() -> bool;
  [[nodiscard]] auto LoadNextLevel() -> bool;
  auto LoadCurrentLevel() -> void;

  [[nodiscard]] auto GetMenuDef() -> menu_def;

  [[nodiscard]] auto GetViewTransform() const -> D2D1::Matrix3x2F;

  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;

  view_type m_view = view_starting;

  int64_t m_totalStartingTicks = 0;
  int64_t m_startingTicks = 0;

  int64_t m_totalEndingTicks = 0;
  int64_t m_endingTicks = 0;

  bool m_paused = false;
  bool m_gameComplete = false;
  bool m_continueRunning = true;

  render_target_view m_levelView;
  std::unique_ptr<level_container> m_levelContainer;

  render_target_view m_overlayView;
  passive_object_container m_overlayContainer;

  game_level_data_loader m_gameLevelDataLoader;

  std::vector<int64_t> m_levelTimes;
};
