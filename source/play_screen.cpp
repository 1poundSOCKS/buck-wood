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
  timer.initialValue = timer.currentValue = performance_counter::QueryValue();
  levelStart = this->timer.initialValue;
  continueRunning = LoadFirstLevel();
}

auto play_screen::Update(const screen_input_state& inputState) -> void
{
  timer.currentValue = performance_counter::QueryValue();
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
    levelTimes.push_back(m_levelTimer->GetTimeRemainingInSeconds());
    
    if( AllLevelsAreComplete() )
    {
      m_gameComplete = true;
      continueRunning = false;
      SetScreenTransitionDelay(3);
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
    pauseStart = timer.currentValue;
  }
  else
  {
    m_levelObjectContainer.Update(timer.currentValue - levelStart - pauseTotal);

    if( m_levelTimer->HasExpired() )
    {
      m_levelState->SetState(level_state::dead);
      SetScreenTransitionDelay(3);
    }
    else if( m_levelObjectContainer.IsComplete() )
    {
      m_levelState->SetState(level_state::complete);
      m_levelTimer->Stop();
      SetScreenTransitionDelay(3);
    }
    else if( player->GetState() == player_ship::dead )
    {
      m_levelState->SetState(level_state::dead);
      m_levelTimer->Stop();
      SetScreenTransitionDelay(3);
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
    pauseTotal += ( timer.currentValue - pauseStart );
  }
}

auto play_screen::UpdateMouseCursorPosition() -> void
{
  m_mouseCursor.SetPosition(renderTargetMouseData.x, renderTargetMouseData.y);
}

auto play_screen::UpdateLevelState(const screen_input_state& inputState) -> void
{
  auto levelControlState = GetLevelControlState(inputState);
  player->SetThruster(levelControlState.thrust);
  player->SetShoot(levelControlState.shoot);

  auto invertedViewTransform = CreateViewTransform(levelControlState.renderTargetMouseData.size);

  if( invertedViewTransform.Invert() )
  {
    D2D1_POINT_2F inPoint { levelControlState.renderTargetMouseData.x, levelControlState.renderTargetMouseData.y };
    auto outPoint = invertedViewTransform.TransformPoint(inPoint);

    mouseX = outPoint.x;
    mouseY = outPoint.y;

    auto playerAngle = CalculateAngle(player->GetXPos(), player->GetYPos(), outPoint.x, outPoint.y);
    player->SetAngle(playerAngle);

    D2D1_POINT_2F screenTopLeft { 0, 0 };
    D2D1_POINT_2F screenBottomRight { levelControlState.renderTargetMouseData.size.width - 1, levelControlState.renderTargetMouseData.size.height - 1 };
    auto viewTopLeft = invertedViewTransform.TransformPoint(screenTopLeft);
    auto viewBottomRight = invertedViewTransform.TransformPoint(screenBottomRight);
    m_viewRect.topLeft = { viewTopLeft.x, viewTopLeft.y };
    m_viewRect.bottomRight = { viewBottomRight.x, viewBottomRight.y };
  }

  m_levelObjectContainer.Update(timer.currentValue - levelStart - pauseTotal);
}

bool play_screen::ScreenTransitionTimeHasExpired()
{
  return timer.currentValue > transitionEnd;
}

void play_screen::SetScreenTransitionDelay(int timeInSeconds)
{
  transitionEnd = timer.currentValue + (timeInSeconds * clock_frequency::get());
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
    levelStart = timer.currentValue;
    return true;
  }
}

auto play_screen::LoadCurrentLevel() -> void
{
  m_levelObjectContainer.Clear();

  m_levelObjectContainer.AppendOverlayObject(m_mouseCursor);

  std::vector<level_island> islands;
  m_gameLevelDataLoader.LoadIslands(std::back_inserter(islands));

  std::for_each(islands.begin(), islands.end(), [this](auto& island)
  {
    m_levelObjectContainer.AppendActiveObject(island);
  });

  std::vector<target_state> targets;
  m_gameLevelDataLoader.LoadTargets(std::back_inserter(targets));

  std::for_each(targets.begin(), targets.end(), [this](auto& target)
  {
    m_levelObjectContainer.AppendActiveObject(target);
  });

  LoadPlayer();

  m_levelState = std::make_unique<level_state>();
  m_levelObjectContainer.AppendOverlayObject(*m_levelState);
  
  m_levelTimer = m_gameLevelDataLoader.LoadTimer();
  m_levelObjectContainer.AppendOverlayObject(*m_levelTimer);
}

level_control_state play_screen::GetLevelControlState(const screen_input_state& inputState)
{
  return {
    inputState.windowData.mouse.rightButtonDown, 
    inputState.windowData.mouse.leftButtonDown, 
    inputState.renderTargetMouseData
  };
}

auto play_screen::LoadPlayer() -> void
{
  player = m_gameLevelDataLoader.LoadPlayer();

  player->SetEventShot([this](float x, float y, float angle) -> void
  {
    m_levelObjectContainer.AppendActiveObject(bullet { x, y, angle });
    playerShot = true;
  });

  player->SetEventDied([this](float x, float y) -> void
  {
  });

  m_levelObjectContainer.AppendActiveObject(*player);
}

[[nodiscard]] auto play_screen::CreateViewTransform(const D2D1_SIZE_F& renderTargetSize, float renderScale) -> D2D1::Matrix3x2F
{
  m_viewTransform = CreateGameLevelTransform(player->GetXPos(), player->GetYPos(), renderScale, renderTargetSize.width, renderTargetSize.height);
  return m_viewTransform;
}

auto play_screen::PlaySoundEffects(const global_sound_buffer_selector& soundBuffers) const -> void
{
  PlaySoundBuffer(soundBuffers[menu_theme], true);

  if( player->ThrusterOn() )
    PlaySoundBuffer(soundBuffers[thrust], true);
  else
    StopSoundBufferPlay(soundBuffers[thrust]);

  if( playerShot )
  {
    ResetSoundBuffer(soundBuffers[shoot]);
    PlaySoundBuffer(soundBuffers[shoot]);
  }

  if( targetShot )
  {
    ResetSoundBuffer(soundBuffers[shoot]);
    PlaySoundBuffer(soundBuffers[target_activated]);
  }
}

[[nodiscard]] auto play_screen::GetMouseDiagnostics() const -> std::wstring
{
  return std::format(L"world mouse: {:.2f}, {:.2f}", mouseX, mouseY);
}
