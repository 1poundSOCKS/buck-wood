#include "pch.h"
#include "play_screen.h"
#include "diagnostics.h"
#include "game_settings.h"
#include "show_level_play_scene.h"
#include "opening_play_scene.h"
#include "main_play_scene.h"
#include "closing_play_scene.h"
#include "hud_target.h"

play_screen::play_screen()
{
  m_menuController.OpenRoot();
  LoadNextLevel();
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
        break;
      case play_menu_controller::selection::quit:
        m_sceneController.Quit();
        break;
    }
  }
  else
  {
    m_sceneController.UpdateScene(ticks);
  }

  return !m_sceneController.Complete();
}

auto play_screen::Render() -> void
{
  render_guard renderGuard { render_target::get() };
  m_sceneController.RenderScene();
  render_target::get()->SetTransform(D2D1::Matrix3x2F::Identity());
  RenderUI();
  RenderDiagnostics();
}

auto play_screen::RenderUI() -> void
{
  if( m_sceneController.Paused() )
  {
    D2D1_SIZE_F renderTargetSize = render_target::get()->GetSize();
    m_menuController.Render(D2D1_RECT_F { 0, 0, renderTargetSize.width - 1, renderTargetSize.height - 1});
  }
  else
  {
    auto currentScene = static_cast<play_scene&>(m_sceneController.Current());
    auto renderTransform = currentScene.RenderTransform();
    auto targettedObject = currentScene.LevelContainer()->TargettedObject();

    if( targettedObject )
    {
      auto screenPosition = renderTransform.TransformPoint( { targettedObject->Position().x, targettedObject->Position().y } );
      hud_target hudTarget { game_point { screenPosition.x, screenPosition.y } };
      renderer::render(hudTarget);
    }
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

auto play_screen::LoadNextLevel() -> bool
{
  if( m_gameLevelDataLoader.NextLevel() )
  {
    play_scene::level_container_ptr levelContainer = m_gameLevelDataLoader.LoadLevel();

    m_sceneController.Clear();
    #ifdef PREVIEW_LEVEL
    m_sceneController.AddScene<show_level_play_scene>(levelContainer);
    #endif
    m_sceneController.AddScene<opening_play_scene>(levelContainer, m_playEvents);
    m_sceneController.AddScene<main_play_scene>(levelContainer, m_playEvents);
    m_sceneController.AddScene<closing_play_scene>(levelContainer, m_playEvents);
    m_sceneController.Begin();

    return true;
  }
  else
  {
    return false;
  }
}

[[nodiscard]] auto play_screen::PausePressed() -> bool
{
  return keyboard_reader::pressed(DIK_ESCAPE) || gamepad_reader::pressed(XINPUT_GAMEPAD_BACK);
}
