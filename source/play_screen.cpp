#include "pch.h"
#include "play_screen.h"
#include "render.h"
#include "diagnostics.h"
#include "screen_view.h"
#include "global_state.h"
#include "clock_frequency.h"

play_screen::play_screen()
{
}

auto play_screen::Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();
  m_dwriteFactory.attach(dwriteFactory);
  m_dwriteFactory->AddRef();
  continueRunning = LoadFirstLevel();
}

auto play_screen::Update(const screen_input_state& inputState) -> void
{
  m_elapsedTicks = m_timer.Update(performance_counter::QueryValue());
  m_levelRemainingTicks = m_levelStopwatch.Update(m_elapsedTicks);

  renderTargetMouseData = inputState.renderTargetMouseData;
  playerShot = targetShot = false;

  UpdateMouseCursorPosition();

  if( m_levelState->GetState() == level_state::paused )
  {
    OnGamePaused(inputState);
  }
  else
  {
    UpdateLevelState(inputState);

    if( ScreenTransitionTimeHasExpired() )
      OnGameRunning(inputState);
  }
}

auto play_screen::Render() const -> void
{
  m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
  m_levelObjectContainer.Render(m_viewTransform);
}

auto play_screen::PlaySoundEffects() const -> void
{
  const auto soundBuffers = global_sound_buffer_selector { sound_data::soundBuffers() };

  if( m_levelState->GetState() == level_state::playing )
  {
    PlaySoundEffects(soundBuffers);
  }
  else
  {
    StopSoundBufferPlay(soundBuffers[menu_theme]);
    StopSoundBufferPlay(soundBuffers[thrust]);
  }
}

[[nodiscard]] auto play_screen::ContinueRunning() const -> bool
{
  return continueRunning;
}

auto play_screen::FormatDiagnostics(diagnostics_data_inserter_type diagnosticsDataInserter) const -> void
{
  diagnosticsDataInserter = GetMouseDiagnostics();
}

void play_screen::OnGameRunning(const screen_input_state& inputState)
{
  if( m_levelState->GetState() == level_state::playing )
  {
    OnGamePlaying(inputState);
  }
  else if( m_levelState->GetState() == level_state::complete )
  {
    levelTimes.push_back(static_cast<float>(m_levelRemainingTicks) / static_cast<float>(clock_frequency::get()));
    
    if( AllLevelsAreComplete() )
    {
      m_gameComplete = true;
      continueRunning = false;
      m_transitionStopwatch.Start(3 * clock_frequency::get());
    }
    else
    {
      if( !LoadNextLevel() )
        m_gameComplete = true;
    }
  }
  else if( m_gameComplete )
  {
    continueRunning = false;
  }
  else if( m_levelState->GetState() == level_state::dead )
  {
    continueRunning = false;
  }
}

void play_screen::OnGamePlaying(const screen_input_state& inputState)
{
  if( KeyPressed(inputState, DIK_ESCAPE) )
  {
    m_levelState->SetState(level_state::paused);
    m_timer.Pause();
  }
  else
  {
    m_levelObjectContainer.Update(m_elapsedTicks);

    if( m_levelRemainingTicks == 0 )
    {
      m_levelState->SetState(level_state::dead);
      m_transitionStopwatch.Start(3 * clock_frequency::get());
    }
    else if( m_levelObjectContainer.IsComplete() )
    {
      m_levelState->SetState(level_state::complete);
      m_timer.Stop();
      m_transitionStopwatch.Start(3 * clock_frequency::get());
    }
    else if( player->GetState() == player_ship::dead )
    {
      m_levelState->SetState(level_state::dead);
      m_timer.Stop();
      m_transitionStopwatch.Start(3 * clock_frequency::get());
    }
  }
}

void play_screen::OnGamePaused(const screen_input_state& inputState)
{
  if( KeyPressed(inputState, DIK_Q) )
  {
    continueRunning = false;
  }
  else if( KeyPressed(inputState, DIK_ESCAPE) )
  {
    m_levelState->SetState(level_state::playing);
    m_timer.Unpause();
  }
}

auto play_screen::UpdateMouseCursorPosition() -> void
{
  m_mouseCursor.SetPosition(renderTargetMouseData.x, renderTargetMouseData.y);
}

auto play_screen::UpdateLevelState(const screen_input_state& inputState) -> void
{
  auto levelControlState = GetLevelControlState(inputState);

  m_playerControlData->SetThruster(levelControlState.thrust);
  m_playerControlData->SetTrigger(levelControlState.shoot);

  auto invertedViewTransform = CreateViewTransform(levelControlState.renderTargetMouseData.size);

  if( invertedViewTransform.Invert() )
  {
    D2D1_POINT_2F inPoint { levelControlState.renderTargetMouseData.x, levelControlState.renderTargetMouseData.y };
    auto outPoint = invertedViewTransform.TransformPoint(inPoint);

    mouseX = outPoint.x;
    mouseY = outPoint.y;

    auto playerPosition = m_playerControlData->GetPosition();
    auto playerAngle = CalculateAngle(playerPosition.x, playerPosition.y, outPoint.x, outPoint.y);
    m_playerControlData->SetAngle(playerAngle);

    D2D1_POINT_2F screenTopLeft { 0, 0 };
    D2D1_POINT_2F screenBottomRight { levelControlState.renderTargetMouseData.size.width - 1, levelControlState.renderTargetMouseData.size.height - 1 };
    auto viewTopLeft = invertedViewTransform.TransformPoint(screenTopLeft);
    auto viewBottomRight = invertedViewTransform.TransformPoint(screenBottomRight);
    m_viewRect.topLeft = { viewTopLeft.x, viewTopLeft.y };
    m_viewRect.bottomRight = { viewBottomRight.x, viewBottomRight.y };
  }

  m_levelObjectContainer.Update(m_elapsedTicks);
}

bool play_screen::ScreenTransitionTimeHasExpired()
{
  return m_transitionRemainingTicks == 0;
}

bool play_screen::AllLevelsAreComplete()
{
  return m_gameLevelDataLoader.EndOfLevels();
}

[[nodiscard]] auto play_screen::LoadFirstLevel() -> bool
{
  if( m_gameLevelDataLoader.EndOfLevels() )
  {
    return false;
  }
  else
  {
    m_levelObjectContainer.Initialize(m_renderTarget.get(), m_dwriteFactory.get());
    LoadCurrentLevel();
    return true;
  }
}

[[nodiscard]] auto play_screen::LoadNextLevel() -> bool
{
  if( m_gameLevelDataLoader.EndOfLevels() )
  {
    return false;
  }
  else
  {
    m_gameLevelDataLoader.NextLevel();
    LoadCurrentLevel();
    return true;
  }
}

auto play_screen::LoadCurrentLevel() -> void
{
  m_levelObjectContainer.Clear();

  m_levelObjectContainer.AppendOverlayObject(m_mouseCursor);

  m_gameLevelDataLoader.LoadIslands(m_levelObjectContainer);
  m_gameLevelDataLoader.LoadTargets(m_levelObjectContainer);

  m_playerControlData = m_gameLevelDataLoader.LoadPlayer(m_levelObjectContainer);
  m_timerControlData = m_gameLevelDataLoader.LoadTimer(m_levelObjectContainer);
  m_stateControlData = m_gameLevelDataLoader.LoadState(m_levelObjectContainer);

  m_playerControlData->SetEventShot([this](float x, float y, float angle) -> void
  {
    m_levelObjectContainer.AppendActiveObject(bullet { x, y, angle });
    m_playerShot = true;
  });

  m_playerControlData->SetEventDied([this](float x, float y) -> void
  {
  });

  m_timer.Start(performance_counter::QueryValue());
  m_levelStopwatch.Start(m_gameLevelDataLoader.GetTimeLimit() * clock_frequency::get());
}

level_control_state play_screen::GetLevelControlState(const screen_input_state& inputState)
{
  return {
    inputState.windowData.mouse.rightButtonDown, 
    inputState.windowData.mouse.leftButtonDown, 
    inputState.renderTargetMouseData
  };
}

[[nodiscard]] auto play_screen::CreateViewTransform(const D2D1_SIZE_F& renderTargetSize, float renderScale) -> D2D1::Matrix3x2F
{
  auto playerPosition = m_playerControlData->GetPosition();
  m_viewTransform = CreateGameLevelTransform(playerPosition.x, playerPosition.y, renderScale, renderTargetSize.width, renderTargetSize.height);
  return m_viewTransform;
}

auto play_screen::PlaySoundEffects(const global_sound_buffer_selector& soundBuffers) const -> void
{
  PlaySoundBuffer(soundBuffers[menu_theme], true);

  if( m_playerControlData->ThrusterOn() )
    PlaySoundBuffer(soundBuffers[thrust], true);
  else
    StopSoundBufferPlay(soundBuffers[thrust]);

  if( m_playerShot )
  {
    ResetSoundBuffer(soundBuffers[shoot]);
    PlaySoundBuffer(soundBuffers[shoot]);
  }

  if( m_targetActivated )
  {
    ResetSoundBuffer(soundBuffers[shoot]);
    PlaySoundBuffer(soundBuffers[target_activated]);
  }
}

[[nodiscard]] auto play_screen::GetMouseDiagnostics() const -> std::wstring
{
  return std::format(L"world mouse: {:.2f}, {:.2f}", mouseX, mouseY);
}
