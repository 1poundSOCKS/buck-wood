#pragma once

#include "diagnostics.h"
#include "passive_object_container.h"
#include "screen_transform.h"
#include "button_def.h"
#include "menu_def.h"

class main_menu_screen
{
public:

  main_menu_screen();
  auto Update(const screen_input_state& inputState, int64_t frameInterval) -> void;
  auto Render() const -> void;
  auto PostPresent() const -> void;
  [[nodiscard]] auto ContinueRunning() const -> bool;
  auto FormatDiagnostics(diagnostics_data_inserter_type diagnosticsDataInserter) const -> void;

private:

  [[nodiscard]] auto GetMenuDef() -> menu_def;

  winrt::com_ptr<ID2D1SolidColorBrush> m_mouseCursorBrush;
  winrt::com_ptr<ID2D1SolidColorBrush> m_menuTextBrush;
  winrt::com_ptr<IDWriteTextFormat> m_menuTextFormat;

  passive_object_container m_objectContainer;
  
  bool m_continueRunning = true;
  bool m_startPlay = false;
};
