#include "pch.h"
#include "main_menu_screen.h"
#include "screen_render.h"
#include "render_brush_defs.h"
#include "render_text_format_def.h"
#include "play_screen.h"
#include "global_state.h"
#include "button.h"
#include "dwrite_factory.h"

inline auto render_text_format_main_menu = render_text_format_def(L"Verdana", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20);

main_menu_screen::main_menu_screen()
{
}

auto main_menu_screen::Initialize(ID2D1RenderTarget* renderTarget) -> void
{
  auto dwriteFactory = dwrite_factory::get().get();

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
  m_objectContainer.Initialize(renderTarget);

  m_objectContainer.AppendOverlayObject(GetMenuDef().CreateMenu());

  // auto buttonDefs = GetMenuDef().GetButtonDefs();
  // for( const auto& buttonDef : buttonDefs )
  // {
  //   m_objectContainer.AppendOverlayObject(buttonDef.CreateButton());
  // }

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

[[nodiscard]] auto main_menu_screen::GetStartButtonDef(std::function<void()> eventClicked) const -> button_def
{
  auto renderTargetSize = m_renderTarget->GetSize();

  auto buttonWidth = renderTargetSize.width / 3.0f;
  auto startButtonHeight = buttonWidth / 4.0f;

  auto buttonLeft = (renderTargetSize.width - buttonWidth) / 2.0f;
  auto buttonRight = buttonLeft + buttonWidth;

  auto startButtonTop = (renderTargetSize.height - startButtonHeight) / 2.0f;
  auto startButtonBottom = startButtonTop + startButtonHeight;

  return button_def(L"Start", false, { buttonLeft, startButtonTop, buttonRight, startButtonBottom }, eventClicked);
}

[[nodiscard]] auto main_menu_screen::GetExitButtonDef(std::function<void()> eventClicked) const -> button_def
{
  auto renderTargetSize = m_renderTarget->GetSize();

  auto buttonWidth = renderTargetSize.width / 3.0f;
  auto exitButtonHeight = buttonWidth / 8.0f;

  auto buttonLeft = (renderTargetSize.width - buttonWidth) / 2.0f;
  auto buttonRight = buttonLeft + buttonWidth;

  auto exitButtonTop = (renderTargetSize.height - exitButtonHeight) / 1.5f;
  auto exitButtonBottom = exitButtonTop + exitButtonHeight;

  return button_def(L"Exit", false, { buttonLeft, exitButtonTop, buttonRight, exitButtonBottom }, eventClicked);
}

[[nodiscard]] auto main_menu_screen::GetMenuDef() -> menu_def
{
  menu_def menuDef(GetRenderTargetArea(m_renderTarget.get(), 0.5f, 0.5f));

  menuDef.AddButtonDef({ L"Start", false, [this]() -> void
  {
    m_startPlay = true;
  }});

  menuDef.AddButtonDef({ L"Exit", false, [this]() -> void
  {
    m_continueRunning = false;
  }});

  menuDef.UpdateButtons();

  return menuDef;
}

[[nodiscard]] auto main_menu_screen::GetRenderTargetArea(ID2D1RenderTarget* renderTarget, float width, float height) -> D2D1_RECT_F
{
  auto renderTargetSize = m_renderTarget->GetSize();
  auto borderWidth = renderTargetSize.width * (1.0f - width) / 2.0f;
  auto borderHeight = renderTargetSize.height * (1.0f - height) / 2.0f;
  return { borderWidth, borderHeight, renderTargetSize.width - borderWidth - 1, renderTargetSize.height - borderHeight - 1 };
}
