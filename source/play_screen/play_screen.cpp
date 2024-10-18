#include "pch.h"
#include "play_screen.h"
#include "diagnostics.h"
#include "game_settings.h"
#include "opening_play_scene.h"
#include "closing_play_scene.h"
#include "hud_target.h"

play_screen::play_screen() : m_playState { std::make_shared<play_state>() }, m_currentScene { std::make_unique<main_play_scene>(m_playState) }
{
  m_menuController.OpenRoot();
  m_playState->LoadCurrentLevel();
  m_currentScene->Begin();
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
    if( !Paused() )
    {
      m_currentScene->Pause();
    }
    
    TogglePause();
  }

  if( Paused() )
  {
    m_menuController.Update();

    switch( m_menuController.Selection() )
    {
      case play_menu_controller::selection::resume:
        TogglePause();
        m_currentScene->Resume();
        break;
      case play_menu_controller::selection::quit:
        m_currentScene->End();
        return false;
      default:
        return true;
    }
  }

  scene_type nextSceneType = m_currentSceneType;

  switch( m_currentSceneType )
  {
    case scene_type::main:
      nextSceneType = m_currentScene->Update(ticks) ? scene_type::main : scene_type::closing;
      break;
    case scene_type::closing:
      nextSceneType = m_currentScene->Update(ticks) ? scene_type::closing : scene_type::main;
      break;
  }

  nextSceneType = ( nextSceneType == scene_type::main && m_playState->Status() == play_state::status::end_of_game ) ? scene_type::none : nextSceneType;

  if( nextSceneType == scene_type::none )
  {
    m_currentSceneType = nextSceneType;
    return false;
  }
  else
  {
    if( nextSceneType != m_currentSceneType )
    {
      m_currentScene->End();

      switch( nextSceneType )
      {
        case scene_type::closing:
          m_currentScene = std::make_unique<closing_play_scene>(m_playState);
          m_currentScene->Begin();
          break;
        case scene_type::main:
          m_currentScene = std::make_unique<main_play_scene>(m_playState);
          m_currentScene->Begin();
          break;
      }

      m_currentSceneType = nextSceneType;
    }

    return true;
  }
}

auto play_screen::Render() -> void
{
  render_guard renderGuard { render_target::get() };

  m_currentScene->Render();

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
