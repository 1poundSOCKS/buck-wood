#include "pch.h"
#include "play_screen.h"
#include "diagnostics.h"
#include "game_settings.h"
#include "opening_play_scene.h"
#include "main_play_scene.h"
#include "closing_play_scene.h"
#include "hud_target.h"

play_screen::play_screen() : 
  m_playState { std::make_shared<play_state>() }
{
  m_menuController.OpenRoot();

  if( m_playState->LoadLevel() )
  {
    CreateScenes();
    m_sceneController.Begin();
  }
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
    m_sceneController.TogglePause();
  }

  if( m_sceneController.Paused() )
  {
    m_menuController.Update();

    switch( m_menuController.Selection() )
    {
      case play_menu_controller::selection::resume:
        m_sceneController.Resume();
        return true;
      case play_menu_controller::selection::quit:
        m_sceneController.Quit();
        return false;
      default:
        return true;
    }
  }

  m_sceneController.UpdateScene(ticks);

  if( m_sceneController.Complete() && m_playState->LoadLevel() )
  {
    m_sceneController.Clear();
    CreateScenes();
    m_sceneController.Begin();
  }

  return m_sceneController.Complete() ? false : true;
}

auto play_screen::Render() -> void
{
  render_guard renderGuard { render_target::get() };

  m_sceneController.RenderScene();

  if( m_sceneController.Paused() )
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

auto play_screen::CreateScenes() -> void
{
  m_sceneController.AddScene<opening_play_scene>(m_playState);
  m_sceneController.AddScene<main_play_scene>(m_playState);
  m_sceneController.AddScene<closing_play_scene>(m_playState);
}

[[nodiscard]] auto play_screen::PausePressed() -> bool
{
  return keyboard_reader::pressed(DIK_ESCAPE) || gamepad_reader::button_pressed(XINPUT_GAMEPAD_BACK);
}
