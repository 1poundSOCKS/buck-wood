#pragma once

#include "diagnostics.h"
#include "object_container_view.h"
#include "button_def.h"

class main_menu_screen
{
public:

  main_menu_screen();

  auto Initialize(ID2D1RenderTarget* renderTarget) -> void;
  auto Update(const screen_input_state& inputState) -> void;
  auto Render() const -> void;
  auto PlaySoundEffects() const -> void;
  [[nodiscard]] auto ContinueRunning() const -> bool;
  auto FormatDiagnostics(diagnostics_data_inserter_type diagnosticsDataInserter) const -> void;

private:

  auto OnViewDefault(const screen_input_state& inputState) -> void;
  auto UpdateScreenExitState(const screen_input_state& screenInputState) -> void;

  [[nodiscard]] auto GetStartButtonDef() -> button_def;
  [[nodiscard]] auto GetExitButtonDef() -> button_def;

  enum view_type { view_default, view_exit };
  view_type m_view = view_default;

  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<ID2D1SolidColorBrush> m_mouseCursorBrush;
  winrt::com_ptr<ID2D1SolidColorBrush> m_menuTextBrush;
  winrt::com_ptr<IDWriteTextFormat> m_menuTextFormat;

  render_target_mouse_data m_renderTargetMouseData;
  object_container_view m_containerView;
  passive_object_container m_objectContainer;
  
  bool m_continueRunning = true;
  bool m_startPlay = false;
  bool m_saveGameLevelData = false;
  bool m_checkSaveOnExit = false;
};
