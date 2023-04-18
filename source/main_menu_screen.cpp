#include "pch.h"
#include "main_menu_screen.h"
#include "screen_render.h"
#include "render_defs.h"

main_menu_screen::main_menu_screen()
{
}

auto main_menu_screen::Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void
{
  m_mouseCursorBrush = CreateScreenRenderBrush(renderTarget, screen_render_brush_white);
  m_menuTextBrush = CreateScreenRenderBrush(renderTarget, screen_render_brush_cyan);
  m_menuTextFormat = CreateScreenRenderTextFormat(dwriteFactory, L"Verdana", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 30);
}

auto main_menu_screen::Update(const screen_input_state& inputState) -> void
{
  if( viewState == view_exit )
  {
    UpdateScreenExitState(inputState);
    return;
  }

  if( KeyPressed(inputState, DIK_ESCAPE) )
  {
    if( checkSaveOnExit )
      viewState = view_exit;
    else
      quit = true;
    
    return;
  }

  renderTargetMouseData = inputState.renderTargetMouseData;
  
  startPlay = startLevelEdit = false;
  
  if( starting )
  {
    starting = false;
  }
  else if( KeyPressed(inputState, DIK_SPACE) )
  {
    startPlay = true;
  }
  else if( KeyPressed(inputState, DIK_F1) )
  {
    startLevelEdit = true;
  }
}

auto main_menu_screen::RenderTo(ID2D1RenderTarget* renderTarget) const -> void
{
  renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  if( viewState == view_exit )
  {
    RenderText(renderTarget, m_menuTextBrush.get(), m_menuTextFormat.get(), L"save changes (y/n)", 
      DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
  }
  else
  {
    std::wstring titleText;
    titleText += L"Right mouse button - accelerate\n";
    titleText += L"Left mouse button - shoot\n";
    titleText += L"\nPress SPACE to start";

    RenderText(renderTarget, m_menuTextBrush.get(), m_menuTextFormat.get(), titleText, 
      DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
    
    RenderMouseCursor(renderTarget, m_mouseCursorBrush.get(), renderTargetMouseData.x, renderTargetMouseData.y);
  }
}

auto main_menu_screen::PlaySoundEffects() const -> void
{
}

[[nodiscard]] auto main_menu_screen::ContinueRunning() const -> bool
{
  return quit || startPlay || startLevelEdit ? false : true;
}

auto main_menu_screen::FormatDiagnostics(diagnostics_data_inserter_type diagnosticsDataInserter) const -> void
{
}

auto main_menu_screen::StartPlay() const -> bool
{
  return startPlay;
}

auto main_menu_screen::StartLevelEditor() const -> bool
{
  return startLevelEdit;
}

auto main_menu_screen::SaveGameLevelData() const -> bool
{
  return saveGameLevelData;
}

auto main_menu_screen::UpdateScreenExitState(const screen_input_state& screenInputState) -> void
{
  if( KeyPressed(screenInputState, DIK_ESCAPE) )
  {
    viewState = view_default;
  }
  else if( KeyPressed(screenInputState, DIK_N) )
  {
    saveGameLevelData = false;
    quit = true;
  }
  else if( KeyPressed(screenInputState, DIK_Y) )
  {
    saveGameLevelData = true;
    quit = true;
  }
}
