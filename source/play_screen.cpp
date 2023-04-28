#include "pch.h"
#include "play_screen.h"
#include "render.h"
#include "diagnostics.h"
#include "screen_view.h"
#include "global_state.h"
#include "render_target_area.h"

play_screen::play_screen()
{
}

auto play_screen::Initialize(ID2D1RenderTarget* renderTarget) -> void
{
  auto dwriteFactory = dwrite_factory::get().get();

  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();
  m_dwriteFactory.attach(dwriteFactory);
  m_dwriteFactory->AddRef();

  m_levelContainer.Initialize(renderTarget);
  m_overlayContainer.Initialize(renderTarget);

  m_levelView.Initialize(renderTarget);
  m_overlayView.Initialize(renderTarget);

  m_continueRunning = LoadFirstLevel();

  auto menu = GetMenuDef().CreateMenu();

  menu.SetCallbackForHiddenFlag([this]() -> bool
  {
    return !m_paused;
  });
  
  m_overlayContainer.AppendOverlayObject(menu);

  m_overlayContainer.AppendOverlayObject(mouse_cursor {});
}

auto play_screen::Update(const screen_input_state& inputState) -> void
{
  m_playerShot = m_targetActivated = false;

  if( PausePressed(inputState) )
    m_paused = !m_paused;

  if( m_paused )
  {
    m_levelControlData.GetStateControl()->SetState(level_state::control::paused);
  }
  else
  {
    m_levelControlData.GetStateControl()->SetState(level_state::control::playing);
  }

  auto elapsedTicks = m_paused ? 0 : performance_counter::QueryFrequency() / framework::fps();

  if( QuitPressed(inputState) )
    m_continueRunning = false;

  if( elapsedTicks > 0 )
  {
    UpdateLevelState(inputState, elapsedTicks);
    m_levelContainer.DoCollisions();
    m_levelContainer.ClearDestroyedObjects();
  }
  
  m_overlayView.Update(m_overlayContainer, inputState, elapsedTicks);

  if( m_levelContainer.IsComplete() )
    m_continueRunning = false;

  auto levelRemainingTime = m_levelControlData.GetTimerControl()->GetValue();

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

    if( m_levelControlData.GetPlayerControl()->ThrusterOn() )
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
  auto playerPosition = m_levelControlData.GetPlayerControl()->GetPosition();
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
  m_levelContainer.ClearAll();
  m_levelControlData = m_gameLevelDataLoader.LoadLevel(m_levelContainer, m_overlayContainer, [this]()
  {
    m_targetActivated = true;
  });

  m_levelControlData.GetPlayerControl()->SetEventShot([this](float x, float y, float angle) -> void
  {
    m_levelContainer.AppendActiveObject(bullet { x, y, angle });
    m_playerShot = true;
  });

  m_levelControlData.GetPlayerControl()->SetEventDied([this](float x, float y) -> void
  {
    m_continueRunning = false;
  });
}

[[nodiscard]] auto play_screen::GetMenuDef() -> menu_def
{
  auto menuArea = render_target_area(m_renderTarget.get(), 0.5f, 0.5f);

  menu_def menuDef(menuArea.GetRect());

  menuDef.AddButtonDef({ L"Resume", [this]() -> void
  {
    m_paused = false;
  }});

  menuDef.AddButtonDef({ L"Quit", [this]() -> void
  {
    m_continueRunning = false;
  }});

  menuDef.UpdateButtons();

  return menuDef;
}
