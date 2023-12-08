#include "pch.h"
#include "play_screen.h"
#include "screen_view.h"
#include "render_target_area.h"
#include "diagnostics.h"
#include "game_settings.h"
#include "show_level_play_scene.h"
#include "opening_play_scene.h"
#include "main_play_scene.h"
#include "closing_play_scene.h"

play_screen::play_screen() : m_levelContainer { play_scene::create_level_container() }
{
  m_menuController.OpenRoot();

  if( LoadNextLevel() )
  {
    RestartScenes();
  }
}

auto play_screen::Refresh(int64_t ticks) -> bool
{
  Update(ticks);
  Render();
  return m_continueRunning && !m_sceneController.Complete();
}

auto play_screen::Update(int64_t ticks) -> void
{
  if( Paused() )
  {
    m_menuController.Update();

    switch( m_menuController.Selection() )
    {
      case play_menu_controller::selection::resume:
        Resume();
        break;
      case play_menu_controller::selection::quit:
        Quit();
        break;
    }
  }

  if( PausePressed() )
  {
    Paused() ? Resume() : Pause();
  }

  auto elapsedTicks = Paused() ? 0 : ticks;
  m_sceneController.UpdateScene(elapsedTicks);
}

auto play_screen::Render() -> void
{
  render_guard renderGuard { render_target::get() };
  m_sceneController.RenderScene();
  RenderUI();
  RenderDiagnostics();
}

auto play_screen::RenderUI() -> void
{
  render_target::get()->SetTransform(D2D1::Matrix3x2F::Identity());

  if( Paused() )
  {
    D2D1_SIZE_F renderTargetSize = render_target::get()->GetSize();
    m_menuController.Render(D2D1_RECT_F { 0, 0, renderTargetSize.width - 1, renderTargetSize.height - 1});
  }
}

auto play_screen::RenderDiagnostics() -> void
{
  diagnostics::addWindowData(main_window::data());
  diagnostics::updateFrameData();

  auto framerate = game_settings::framerate();

  if( framerate )
  {
    diagnostics::addTimingData(*framerate);
  }

  renderer::renderDiagnostics();
  diagnostics::clear();
}

[[nodiscard]] auto play_screen::PausePressed() -> bool
{
  return keyboard_reader::pressed(DIK_ESCAPE) || gamepad_reader::pressed(XINPUT_GAMEPAD_BACK);
}

[[nodiscard]] auto play_screen::Paused() const -> bool
{
  return m_paused;
}

auto play_screen::Pause() -> void
{
  m_sceneController.Pause();
  m_paused = true;
}

auto play_screen::Resume() -> void
{
  m_sceneController.Resume();
  m_paused = false;
}

auto play_screen::Quit() -> void
{
  m_sceneController.End();
  m_continueRunning = false;
}

auto play_screen::LoadNextLevel() -> bool
{
  if( m_gameLevelDataLoader.NextLevel() )
  {
    m_levelContainer = m_gameLevelDataLoader.LoadLevel();
    return true;
  }
  else
  {
    return false;
  }
}

auto play_screen::RestartScenes() -> void
{
  m_sceneController.Clear();

  #ifdef PREVIEW_LEVEL
  m_sceneController.AddScene<show_level_play_scene>(m_levelContainer);
  #endif

  m_sceneController.AddScene<opening_play_scene>(m_levelContainer);
  m_sceneController.AddScene<main_play_scene>(m_levelContainer);
  m_sceneController.AddScene<closing_play_scene>(m_levelContainer);
  m_sceneController.Begin();
}