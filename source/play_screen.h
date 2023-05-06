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
#include "play_screen_view.h"

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

  [[nodiscard]] auto PausePressed(const screen_input_state& inputState) -> bool;
  [[nodiscard]] auto LoadFirstLevel() -> bool;
  [[nodiscard]] auto LoadNextLevel() -> bool;
  auto LoadCurrentLevel() -> void;
  [[nodiscard]] auto GetMenuDef() -> menu_def;

  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;

  play_screen_view m_screenView;

  bool m_paused = false;
  bool m_continueRunning = true;

  // screen_transform m_levelTransform;
  std::unique_ptr<level_container> m_levelContainer;

  screen_transform m_overlayTransform;
  passive_object_container m_overlayContainer;

  game_level_data_loader m_gameLevelDataLoader;

  std::vector<int64_t> m_levelTimes;
};
