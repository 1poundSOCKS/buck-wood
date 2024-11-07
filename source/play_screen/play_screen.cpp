#include "pch.h"
#include "play_screen.h"
#include "diagnostics.h"
#include "game_settings.h"
#include "opening_play_scene.h"
#include "main_play_scene.h"
#include "closing_play_scene.h"
#include "final_play_scene.h"
#include "game_state.h"

play_screen::play_screen() : 
  m_playState { std::make_shared<play_state>() }, 
  m_currentScene { std::make_unique<opening_play_scene>(m_playState) },
  m_currentSceneType { scene_type::opening }
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
    TogglePause();
    
    if( Paused() )
    {
      m_currentScene->Pause();
    }
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

  return m_currentScene->Update(ticks) ? true : NextScene();
}

auto play_screen::NextScene() -> bool
{
  m_currentScene->End();

  switch( m_currentSceneType )
  {
    case scene_type::opening:
      m_currentScene = std::make_unique<main_play_scene>(m_playState);
      m_currentScene->Begin();
      m_currentSceneType = scene_type::main;
      return true;

    case scene_type::main:
      m_currentScene = std::make_unique<closing_play_scene>(m_playState);
      m_currentScene->Begin();
      m_currentSceneType = scene_type::closing;
      return true;
      
    case scene_type::closing:
      return m_playState->GameOver() ? RestartLevel() : StartNextLevel();

    default:
      return false;
  }
}

auto play_screen::StartNextLevel() -> bool
{
  if( m_playState->LoadNextLevel() )
  {
    m_currentScene = std::make_unique<opening_play_scene>(m_playState);
    m_currentSceneType = scene_type::opening;
    m_currentScene->Begin();
    return true;
  }
  else
  {
    game_state::set_level_index(0);
    return false;
  }
}

auto play_screen::RestartLevel() -> bool
{
  if( m_playState->LoadCurrentLevel() )
  {
    m_currentScene = std::make_unique<main_play_scene>(m_playState);
    m_currentSceneType = scene_type::main;
    m_currentScene->Begin();
    return true;
  }
  else
  {
    game_state::set_level_index(0);
    return false;
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
