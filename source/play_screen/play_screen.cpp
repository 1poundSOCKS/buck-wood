#include "pch.h"
#include "play_screen.h"
#include "diagnostics.h"
#include "game_settings.h"
#include "opening_play_scene.h"
#include "main_play_scene.h"
#include "closing_play_scene.h"
#include "final_play_scene.h"
#include "game_state.h"
#include "game_clock.h"
#include "cell_path.h"

play_screen::play_screen() : 
  m_playState { std::make_shared<play_state>() }, 
  m_levelContainer { std::make_shared<level_container>() },
  m_currentScene { std::make_unique<opening_play_scene>(*m_levelContainer) },
  m_currentSceneType { scene_type::opening }
{
  m_menuController.OpenRoot();
  LoadLevel(m_playState->LevelIndex());
  m_currentScene->Begin(*m_levelContainer);
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
      m_currentScene->Pause(*m_levelContainer);
    }
  }

  if( Paused() )
  {
    m_menuController.Update();

    switch( m_menuController.Selection() )
    {
      case play_menu_controller::selection::resume:
        TogglePause();
        m_currentScene->Resume(*m_levelContainer);
        break;
      case play_menu_controller::selection::quit:
        m_currentScene->End(*m_levelContainer);
        return false;
      default:
        return true;
    }
  }

  PlaySoundEffects();
  play_events::reset();
  m_currentScene->Update(*m_levelContainer, ticks);

  auto interval = m_currentSceneType == scene_type::opening ? 0.0f : game_clock::getInterval(ticks);

  m_playState->Update(interval);

  if( !m_playState->LevelComplete(*m_levelContainer) && m_playState->TimedOut() )
  {
    m_levelContainer->PlayerState().Destroy();
  }

  m_levelContainer->Objects().Visit([this](auto&& object) { VisitObject(object); });

  auto renderView = m_currentScene->GetRenderTargetView(*m_levelContainer);
  m_levelContainer->Update(interval, renderView, m_playState->LevelComplete(*m_levelContainer));

  return m_currentScene->Complete(*m_levelContainer, *m_playState) ? NextScene() : true;
}

auto play_screen::NextScene() -> bool
{
  m_currentScene->End(*m_levelContainer);

  switch( m_currentSceneType )
  {
    case scene_type::opening:
      m_currentScene = std::make_unique<main_play_scene>(*m_levelContainer);
      m_currentScene->Begin(*m_levelContainer);
      m_currentSceneType = scene_type::main;
      return true;

    case scene_type::main:
      m_currentScene = std::make_unique<closing_play_scene>(*m_levelContainer);
      m_currentScene->Begin(*m_levelContainer);
      m_currentSceneType = scene_type::closing;
      return true;
      
    case scene_type::closing:
      return m_playState->GameOver(*m_levelContainer) ? RestartLevel() : StartNextLevel();

    default:
      return false;
  }
}

auto play_screen::StartNextLevel() -> bool
{
  if( LoadLevel(m_playState->IncrementLevelIndex()) )
  {
    m_currentScene = std::make_unique<opening_play_scene>(*m_levelContainer);
    m_currentSceneType = scene_type::opening;
    m_currentScene->Begin(*m_levelContainer);
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
  if( LoadLevel(m_playState->LevelIndex()) )
  {
    m_currentScene = std::make_unique<main_play_scene>(*m_levelContainer);
    m_currentSceneType = scene_type::main;
    m_currentScene->Begin(*m_levelContainer);
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

  m_currentScene->Render(*m_levelContainer, *m_playState);

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

auto play_screen::PlaySoundEffects() const -> void
{
  if( play_events::get(play_events::event_type::shot) )
  {
    audio_events::PlayerShot();
  }

  if( play_events::get(play_events::event_type::explosion) )
  {
    audio_events::Explosion();
  }

  if( play_events::get(play_events::counter_type::power_ups_collected) )
  {
    audio_events::PowerUpCollected();
  }

  auto&& playerState = m_levelContainer->PlayerState();

  if( !playerState.Destroyed() && playerState.ThrusterPower() > 0.0f )
  {
    audio_events::StartPlayerThruster();
  }
  else
  {
    audio_events::StopPlayerThruster();
  }
}

auto play_screen::LoadLevel(int levelIndex) -> bool
{
  m_playState->Reset();

  if( game_level_data_loader::testLoadLevel(levelIndex) )
  {
    m_levelContainer = std::make_shared<level_container>();
    game_level_data_loader::loadLevel(levelIndex, *m_levelContainer);
    game_state::set_level_index(levelIndex);
    m_emptyCellLookup.clear();
    game_level_data_loader::loadEmptyCellData(levelIndex, std::inserter(m_emptyCellLookup, std::begin(m_emptyCellLookup)));
    log::write(log::type::info, "Load next level successful: index={}", levelIndex);
    return true;
  }
  else
  {
    log::write(log::type::info, "Load next level failed: index={}", levelIndex);
    return false;
  }
}

auto play_screen::VisitObject(enemy_ship &object) const noexcept -> void
{
  auto&& playerState = m_levelContainer->PlayerState();

  if( !playerState.Destroyed() )
  {
    auto playerCell = game_level_data_loader::getCellFromPosition(playerState.Position());
    auto enemyCell = game_level_data_loader::getCellFromPosition(object.Position());
    auto lineOfFireIsClear = CellsAreVisibleToEachOther(playerCell, enemyCell, m_emptyCellLookup);
    object.SetFireStatus(lineOfFireIsClear ? enemy_ship::fire_status::enabled : enemy_ship::fire_status::disabled);
  }
  else
  {
    object.SetFireStatus(enemy_ship::fire_status::disabled);
  }
}

auto play_screen::CellsAreVisibleToEachOther(POINT_2I cellId1, POINT_2I cellId2, const std::set<std::pair<int, int>> &emptyCellLookup) -> bool
{
  cell_path cellPath { cellId1, cellId2 };

  return std::accumulate(std::begin(cellPath), std::end(cellPath), true, [&emptyCellLookup](bool visible, auto&& cellId)
  {
    return visible && emptyCellLookup.contains({cellId.x, cellId.y});
  });
}
