#pragma once

#include "diagnostics.h"

class main_menu_screen
{
public:

  main_menu_screen();

  auto Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void;
  auto Update(const screen_input_state& inputState) -> void;
  auto Render() const -> void;
  auto PlaySoundEffects() const -> void;
  [[nodiscard]] auto ContinueRunning() const -> bool;
  auto FormatDiagnostics(diagnostics_data_inserter_type diagnosticsDataInserter) const -> void;

  auto StartPlay() const -> bool;
  auto StartLevelEditor() const -> bool;
  auto SaveGameLevelData() const -> bool;

private:

  auto UpdateScreenExitState(const screen_input_state& screenInputState) -> void;

  enum view_state { view_default, view_exit };
  view_state viewState = view_default;
  render_target_mouse_data renderTargetMouseData;
  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<ID2D1SolidColorBrush> m_mouseCursorBrush;
  winrt::com_ptr<ID2D1SolidColorBrush> m_menuTextBrush;
  winrt::com_ptr<IDWriteTextFormat> m_menuTextFormat;
  bool starting = true;
  bool running = true;
  bool quit = false;
  bool startPlay = false;
  bool startLevelEdit = false;
  bool saveGameLevelData = false;
  bool checkSaveOnExit = false;
};
