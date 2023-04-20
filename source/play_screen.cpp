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
  m_continueRunning = LoadFirstLevel();
}

auto play_screen::Update(const screen_input_state& inputState) -> void
{
  renderTargetMouseData = inputState.renderTargetMouseData;
  m_playerShot = m_targetActivated = false;

  UpdateMouseCursorPosition();

  if( PausePressed(inputState) )
  {
    m_paused = !m_paused;
  }

  auto elapsedTicks = m_paused ? 0 : performance_counter::QueryFrequency() / framework::fps();
  auto levelRemainingTicks = m_levelStopwatch.Update(elapsedTicks);

  auto levelRemainingTime = static_cast<float>(levelRemainingTicks) / static_cast<float>(performance_counter::QueryFrequency());
  m_timerControlData->SetValue(levelRemainingTime);

  if( levelRemainingTicks == 0 )
  {
    m_continueRunning = false;
  }

  if( QuitPressed(inputState) )
  {
    m_continueRunning = false;
  }

  if( elapsedTicks > 0 )
  {
    UpdateLevelState(inputState, elapsedTicks);
    
    if( m_levelObjectContainer.IsComplete() )
    {
      m_continueRunning = false;
    }
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

  if( m_paused || !m_continueRunning )
  {
    StopSoundBufferPlay(soundBuffers[menu_theme]);
    StopSoundBufferPlay(soundBuffers[thrust]);
  }
  else
  {
    PlaySoundEffects(soundBuffers);
  }
}

[[nodiscard]] auto play_screen::ContinueRunning() const -> bool
{
  return m_continueRunning;
}

auto play_screen::FormatDiagnostics(diagnostics_data_inserter_type diagnosticsDataInserter) const -> void
{
  diagnosticsDataInserter = GetMouseDiagnostics();
}

auto play_screen::UpdateMouseCursorPosition() -> void
{
  m_mouseCursor.SetPosition(renderTargetMouseData.x, renderTargetMouseData.y);
}

auto play_screen::UpdateLevelState(const screen_input_state& inputState, int64_t elapsedTicks) -> void
{
  auto levelControlState = GetLevelControlState(inputState);

  m_playerControlData->SetThruster(levelControlState.thrust);
  m_playerControlData->SetTrigger(levelControlState.shoot);

  auto playerPosition = m_playerControlData->GetPosition();
  m_viewTransform = CreateGameLevelTransform(playerPosition.x, playerPosition.y, 1.4f, levelControlState.renderTargetMouseData.size.width, levelControlState.renderTargetMouseData.size.height);

  auto invertedViewTransform = m_viewTransform;

  if( invertedViewTransform.Invert() )
  {
    m_mouseLevelPosition = invertedViewTransform.TransformPoint({ levelControlState.renderTargetMouseData.x, levelControlState.renderTargetMouseData.y });

    auto playerAngle = CalculateAngle(playerPosition.x, playerPosition.y, m_mouseLevelPosition.x, m_mouseLevelPosition.y);
    m_playerControlData->SetAngle(playerAngle);

    auto viewTopLeft = invertedViewTransform.TransformPoint({ 0, 0 });
    auto viewBottomRight = invertedViewTransform.TransformPoint({ levelControlState.renderTargetMouseData.size.width - 1, levelControlState.renderTargetMouseData.size.height - 1 });
    m_viewRect.topLeft = { viewTopLeft.x, viewTopLeft.y };
    m_viewRect.bottomRight = { viewBottomRight.x, viewBottomRight.y };
  }

  m_levelObjectContainer.Update(elapsedTicks);
}

[[nodiscard]] auto play_screen::PausePressed(const screen_input_state& inputState) -> bool
{
  return KeyPressed(inputState, DIK_ESCAPE);
}

[[nodiscard]] auto play_screen::QuitPressed(const screen_input_state& inputState) -> bool
{
  return KeyPressed(inputState, DIK_Q);
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
    m_continueRunning = false;
  });

  m_levelStopwatch.Start(m_gameLevelDataLoader.GetTimeLimit() * performance_counter::QueryFrequency());
}

level_control_state play_screen::GetLevelControlState(const screen_input_state& inputState)
{
  return {
    inputState.windowData.mouse.rightButtonDown, 
    inputState.windowData.mouse.leftButtonDown, 
    inputState.renderTargetMouseData
  };
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
  return std::format(L"world mouse: {:.2f}, {:.2f}", m_mouseLevelPosition.x, m_mouseLevelPosition.y);
}
