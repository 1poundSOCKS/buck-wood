#include "pch.h"
#include "main_menu_screen.h"
#include "screen_render.h"
#include "render_brush_defs.h"
#include "render_text_format_def.h"
#include "play_screen.h"
#include "global_state.h"
#include "button.h"

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

  m_containerView.Initialize(renderTarget);
  m_objectContainer.Initialize(renderTarget, dwriteFactory);

  auto renderTargetSize = renderTarget->GetSize();

  auto buttonWidth = renderTargetSize.width / 3.0f;
  auto startButtonHeight = buttonWidth / 4.0f;
  auto exitButtonHeight = startButtonHeight / 2.0f;

  auto buttonLeft = (renderTargetSize.width - buttonWidth) / 2.0f;
  auto buttonRight = buttonLeft + buttonWidth;

  auto startButtonTop = (renderTargetSize.height - startButtonHeight) / 2.0f;
  auto startButtonBottom = startButtonTop + startButtonHeight;

  auto exitButtonTop = (renderTargetSize.height - exitButtonHeight) / 1.4f;
  auto exitButtonBottom = exitButtonTop + exitButtonHeight;

  auto startPlay = button { { buttonLeft, startButtonTop, buttonRight, startButtonBottom }, L"START", [this]()
  {
    m_startPlay = true;
  }};

  auto exitGame = button { { buttonLeft, exitButtonTop, buttonRight, exitButtonBottom }, L"EXIT", [this]()
  {
    m_continueRunning = false;
  }};

  m_objectContainer.AppendOverlayObject(startPlay);
  m_objectContainer.AppendOverlayObject(exitGame);
  m_objectContainer.AppendOverlayObject(mouse_cursor{});
}

auto main_menu_screen::Update(const screen_input_state& inputState) -> void
{
  m_containerView.Update(m_objectContainer, inputState, 0);

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
  m_containerView.Render(m_objectContainer);

  if( m_view == view_exit )
  {
    RenderText(m_renderTarget.get(), m_menuTextBrush.get(), m_menuTextFormat.get(), L"save changes (y/n)", DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
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

  if( m_startPlay )
  {    
    play_screen playScreen;
    framework::openScreen(playScreen);
    m_startPlay = false;
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
