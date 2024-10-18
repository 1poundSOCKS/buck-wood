#include "pch.h"
#include "play_screen.h"
#include "diagnostics.h"
#include "game_settings.h"
#include "opening_play_scene.h"
#include "closing_play_scene.h"
#include "hud_target.h"

play_screen::play_screen() : m_playState { std::make_shared<play_state>() }, m_mainPlayScene { m_playState }, m_closingPlayScene { m_playState }
{
  m_menuController.OpenRoot();
  m_playState->LoadCurrentLevel();
  m_mainPlayScene.Begin();
}

auto play_screen::Refresh(int64_t ticks) -> bool
{
  Render();
  return Update(ticks);
}

auto play_screen::Update(int64_t ticks) -> bool
{
  if( PausePressed() )
  {
    TogglePause();
  }

  if( Paused() )
  {
    m_menuController.Update();

    switch( m_menuController.Selection() )
    {
      case play_menu_controller::selection::resume:
        TogglePause();
        break;
      case play_menu_controller::selection::quit:
        return false;
      default:
        return true;
    }
  }

  bool sceneActive = true;

  switch( m_currentScene )
  {
    case scene::main:
      m_currentScene = m_mainPlayScene.Update(ticks) ? scene::main : scene::closing;
      if( m_currentScene == scene::closing )
      {
        m_mainPlayScene.End();
        m_closingPlayScene.Begin();
      }
      break;
    case scene::closing:
      sceneActive = m_closingPlayScene.Update(ticks);
      if( !sceneActive )
      {
        m_closingPlayScene.End();
        m_currentScene = m_playState->Status() == play_state::status::end_of_game ? scene::closing : scene::main;
      }
      break;
  }

  return m_currentScene == scene::main || sceneActive;
}

auto play_screen::Render() -> void
{
  render_guard renderGuard { render_target::get() };

  switch( m_currentScene )
  {
    case scene::main:
      m_mainPlayScene.Render();
      break;
    case scene::closing:
      m_closingPlayScene.Render();
      break;
  }

  if( Paused() )
  {
    render_target::get()->SetTransform(D2D1::Matrix3x2F::Identity());
    D2D1_SIZE_F renderTargetSize = render_target::get()->GetSize();
    m_menuController.Render(D2D1_RECT_F { 0, 0, renderTargetSize.width - 1, renderTargetSize.height - 1});
  }

  if( game_settings::showDiagnostics() )
  {
    render_target::get()->SetTransform(D2D1::Matrix3x2F::Identity());
    RenderDiagnostics();
  }
}

auto play_screen::RenderDiagnostics() -> void
{
  diagnostics::addWindowData(main_window::data());
  diagnostics::updateFrameData();
  diagnostics::addFPS();
  renderer::renderDiagnostics();
  diagnostics::clear();
}

[[nodiscard]] auto play_screen::PausePressed() -> bool
{
  return keyboard_reader::pressed(DIK_ESCAPE) || gamepad_reader::button_pressed(XINPUT_GAMEPAD_BACK);
}

auto play_screen::TogglePause() noexcept -> void
{
  m_paused = !m_paused; 
}

auto play_screen::Paused() const noexcept -> bool
{
  return m_paused;
}
