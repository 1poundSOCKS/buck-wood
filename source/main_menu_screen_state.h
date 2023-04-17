#ifndef _main_menu_screen_state_
#define _main_menu_screen_state_

#include "main_window.h"
#include "screen_render_data.h"
#include "diagnostics.h"

struct main_menu_screen_state
{
public:

  main_menu_screen_state(screen_render_data renderData);

  auto Update(const screen_input_state& inputState) -> void;
  auto RenderTo(ID2D1RenderTarget* renderTarget) const -> void;
  auto PlaySoundEffects() const -> void;
  [[nodiscard]] auto ContinueRunning() const -> bool;
  auto FormatDiagnostics(diagnostics_data_inserter_type diagnosticsDataInserter) const -> void;

  auto StartPlay() const -> bool;
  auto SaveGameLevelData() const -> bool;

private:

  auto UpdateScreenExitState(const screen_input_state& screenInputState) -> void;

  screen_render_data renderData;

  enum view_state { view_default, view_exit };
  view_state viewState = view_default;

  render_target_mouse_data renderTargetMouseData;

  bool starting = true;
  bool running = true;
  bool quit = false;
  bool startPlay = false;
  bool startLevelEdit = false;
  bool saveGameLevelData = false;
  bool checkSaveOnExit = false;
  float mousePointerX = 0;
  float mousePointerY = 0;
};

#endif
