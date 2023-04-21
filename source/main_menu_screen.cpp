#include "pch.h"
#include "main_menu_screen.h"
#include "screen_render.h"
#include "render_brush_defs.h"
#include "render_text_format_def.h"
#include "play_screen.h"
#include "global_state.h"

inline auto render_text_format_main_menu = render_text_format_def(L"Verdana", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20);

main_menu_screen::main_menu_screen()
{
}

auto main_menu_screen::Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();
  m_mouseCursorBrush = screen_render_brush_white.CreateBrush(renderTarget);
  m_menuTextBrush = screen_render_brush_cyan.CreateBrush(renderTarget);
  m_menuTextFormat = render_text_format_main_menu.CreateTextFormat(dwriteFactory);
  config_file::create(L"config.txt");
  const auto& dataPath = config_file::getSetting(L"data_path");
  global_state::create(dataPath);
  sound_data::create(framework::directSound().get(), dataPath);

  // play sound now to ensure no sound glitch on first real play
  {
    global_sound_buffer_selector dummySelector { sound_data::soundBuffers() };
    sound_buffer_player dummyPlayer(dummySelector[menu_theme]);
    dummyPlayer.Play();
  }
}

auto main_menu_screen::Update(const screen_input_state& inputState) -> void
{
  switch( m_view )
  {
    case view_exit:
      UpdateScreenExitState(inputState);
      break;
    default:
      OnViewDefault(inputState);
      break;
  }
}

auto main_menu_screen::Render() const -> void
{
  m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  if( m_view == view_exit )
  {
    RenderText(m_renderTarget.get(), m_menuTextBrush.get(), m_menuTextFormat.get(), L"save changes (y/n)", DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
  }
  else
  {
    std::wstring titleText;
    titleText += L"Right mouse button - accelerate\n";
    titleText += L"Left mouse button - shoot\n";
    titleText += L"\nPress SPACE to start";

    RenderText(m_renderTarget.get(), m_menuTextBrush.get(), m_menuTextFormat.get(), titleText, DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);    
    RenderMouseCursor(m_renderTarget.get(), m_mouseCursorBrush.get(), m_renderTargetMouseData.x, m_renderTargetMouseData.y);
  }
}

auto main_menu_screen::PlaySoundEffects() const -> void
{
}

[[nodiscard]] auto main_menu_screen::ContinueRunning() const -> bool
{
  if( !m_continueRunning && m_saveGameLevelData )
    global_state::save();

  return m_continueRunning;
}

auto main_menu_screen::FormatDiagnostics(diagnostics_data_inserter_type diagnosticsDataInserter) const -> void
{
}

auto main_menu_screen::OnViewDefault(const screen_input_state& inputState) -> void
{
  m_renderTargetMouseData = inputState.renderTargetMouseData;
  
  if( KeyPressed(inputState, DIK_SPACE) )
  {
    play_screen playScreen;
    framework::openScreen(playScreen);
  }
  else if( KeyPressed(inputState, DIK_F1) )
  {
  }
  else if( KeyPressed(inputState, DIK_ESCAPE) )
  {
    if( m_checkSaveOnExit )
    {
      m_view = view_exit;
    }
    else
    {
      m_continueRunning = false;
    }
  }
}

auto main_menu_screen::UpdateScreenExitState(const screen_input_state& screenInputState) -> void
{
  if( KeyPressed(screenInputState, DIK_ESCAPE) )
  {
    m_view = view_default;
  }
  else if( KeyPressed(screenInputState, DIK_N) )
  {
    m_saveGameLevelData = false;
    m_continueRunning = false;
  }
  else if( KeyPressed(screenInputState, DIK_Y) )
  {
    m_saveGameLevelData = true;
    m_continueRunning = true;
  }
}
