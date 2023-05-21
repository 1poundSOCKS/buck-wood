#include "pch.h"
#include "main_menu_screen.h"
#include "screen_render.h"
#include "render_brush_defs.h"
#include "render_text_format_def.h"
#include "play_screen.h"
#include "global_state.h"
#include "button.h"
#include "dwrite_factory.h"
#include "render_target_area.h"
#include "screen_runner.h"

inline auto render_text_format_main_menu = render_text_format_def(L"Verdana", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20);

main_menu_screen::main_menu_screen()
{
  const auto& renderTarget = framework::renderTarget();

  m_mouseCursorBrush = screen_render_brush_white.CreateBrush(renderTarget.get());
  m_menuTextBrush = screen_render_brush_cyan.CreateBrush(renderTarget.get());

  auto dwriteFactory = dwrite_factory::get().get();
  m_menuTextFormat = render_text_format_main_menu.CreateTextFormat(dwriteFactory);
  
  config_file::create(L"config.txt");

  const auto& fullScreen = config_file::getSetting(L"full_screen");
  if( fullScreen == L"true" )
  {
    framework::fullScreen();
  }

  const auto& dataPath = config_file::getSetting(L"data_path");
  global_state::load(dataPath);
  sound_data::create(framework::directSound().get(), dataPath);

  // play sound now to ensure no sound glitch on first real play
  {
    global_sound_buffer_selector dummySelector { sound_data::soundBuffers() };
    sound_buffer_player dummyPlayer(dummySelector[menu_theme]);
    dummyPlayer.Play();
  }

  m_objectContainer += GetMenuDef().CreateMenu();
  m_objectContainer += mouse_cursor {};
}

auto main_menu_screen::Update(const screen_input_state& inputState, int64_t frameInterval) -> void
{
  if( m_startPlay )
  {
    m_startPlay = false;
    OpenScreen<play_screen>();
  }

  screen_transform screenTransform;
  auto inputData = screenTransform.GetObjectInputData(inputState);
  m_objectContainer.Update(inputData, frameInterval);
}

auto main_menu_screen::Render() const -> void
{
  const auto& renderTarget = framework::renderTarget();

  renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  screen_transform screenTransform;
  renderTarget->SetTransform(screenTransform.Get());
  m_objectContainer.Render(screenTransform.GetViewRect(renderTarget->GetSize()));
}

auto main_menu_screen::PostPresent() const -> void
{
}

[[nodiscard]] auto main_menu_screen::ContinueRunning() const -> bool
{
  return m_continueRunning;
}

auto main_menu_screen::FormatDiagnostics(diagnostics_data_inserter_type diagnosticsDataInserter) const -> void
{
}

[[nodiscard]] auto main_menu_screen::GetMenuDef() -> menu_def
{
  const auto& renderTarget = framework::renderTarget();

  auto menuArea = render_target_area(renderTarget->GetSize(), 0.4f, 0.4f);

  menu_def menuDef(menuArea.GetRect());

  menuDef.AddButtonDef({ L"Start", [this]() -> void
  {
    m_startPlay = true;
  }});

  menuDef.AddButtonDef({ L"Exit", [this]() -> void
  {
    m_continueRunning = false;
  }});

  menuDef.UpdateButtons();

  return menuDef;
}
