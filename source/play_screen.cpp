#include "pch.h"
#include "play_screen.h"
#include "render.h"
#include "diagnostics.h"
#include "screen_view.h"
#include "global_state.h"

play_screen::play_screen()
{
}

auto play_screen::Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();
  m_dwriteFactory.attach(dwriteFactory);
  m_dwriteFactory->AddRef();

  m_levelContainer.Initialize(renderTarget, dwriteFactory);
  m_overlayContainer.Initialize(renderTarget, dwriteFactory);

  m_levelView.Initialize(renderTarget);
  m_overlayView.Initialize(renderTarget);

  m_continueRunning = LoadFirstLevel();
}

auto play_screen::Update(const screen_input_state& inputState) -> void
{
  m_playerShot = m_targetActivated = false;

  if( PausePressed(inputState) )
    m_paused = !m_paused;

  auto elapsedTicks = m_paused ? 0 : performance_counter::QueryFrequency() / framework::fps();
  // auto levelRemainingTicks = m_levelStopwatch.Update(elapsedTicks);

  if( QuitPressed(inputState) )
    m_continueRunning = false;

  if( elapsedTicks > 0 )
    UpdateLevelState(inputState, elapsedTicks);
  
  m_overlayView.Update(m_overlayContainer, inputState, elapsedTicks);

  if( m_levelContainer.IsComplete() )
    m_continueRunning = false;

  // auto levelRemainingTime = static_cast<float>(levelRemainingTicks) / static_cast<float>(performance_counter::QueryFrequency());
  auto levelRemainingTime = m_timerControlData->GetValue();

  if( levelRemainingTime == 0 )
    m_continueRunning = false;
}

auto play_screen::Render() const -> void
{
  m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
  m_levelView.Render(m_levelContainer);
  m_overlayView.Render(m_overlayContainer);
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
}

[[nodiscard]] auto play_screen::ContinueRunning() const -> bool
{
  return m_continueRunning;
}

auto play_screen::FormatDiagnostics(diagnostics_data_inserter_type diagnosticsDataInserter) const -> void
{
}

auto play_screen::UpdateLevelState(const screen_input_state& inputState, int64_t elapsedTicks) -> void
{
  auto playerPosition = m_playerControlData->GetPosition();
  auto viewTransform = CreateGameLevelTransform(playerPosition.x, playerPosition.y, 1.4f, inputState.renderTargetMouseData.size.width, inputState.renderTargetMouseData.size.height);
  m_levelView.SetTransform(viewTransform);
  m_levelView.Update(m_levelContainer, inputState, elapsedTicks);
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
  m_levelContainer.Clear();

  m_overlayContainer.AppendOverlayObject(mouse_cursor {});

  m_gameLevelDataLoader.LoadIslands(m_levelContainer);
  m_gameLevelDataLoader.LoadTargets(m_levelContainer);

  m_playerControlData = m_gameLevelDataLoader.LoadPlayer(m_levelContainer);

  m_playerControlData->SetEventShot([this](float x, float y, float angle) -> void
  {
    m_levelContainer.AppendActiveObject(bullet { x, y, angle });
    m_playerShot = true;
  });

  m_playerControlData->SetEventDied([this](float x, float y) -> void
  {
    m_continueRunning = false;
  });

  m_timerControlData = m_gameLevelDataLoader.LoadTimer(m_overlayContainer);
  m_stateControlData = m_gameLevelDataLoader.LoadState(m_overlayContainer);
  // m_levelStopwatch.Start(m_gameLevelDataLoader.GetTimeLimit() * performance_counter::QueryFrequency());
}
