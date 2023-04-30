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

  m_levelContainer.GetObjectContainer().Initialize(renderTarget);
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
  m_overlayContainer.AppendOverlayObject(level_timer { 0 });
  m_overlayContainer.AppendOverlayObject(mouse_cursor {});
}

auto play_screen::Update(const screen_input_state& inputState) -> void
{
  m_playerShot = m_targetActivated = false;

  if( PausePressed(inputState) )
    m_paused = !m_paused;

  auto elapsedTicks = m_paused ? 0 : performance_counter::QueryFrequency() / framework::fps();

  if( QuitPressed(inputState) )
    m_continueRunning = false;

  if( elapsedTicks > 0 )
  {
    auto viewTransform = CreateGameLevelTransform(m_levelViewCentreX, m_levelViewCentreY, 1.5f, 
      inputState.renderTargetMouseData.size.width, inputState.renderTargetMouseData.size.height);
    
    m_levelView.SetTransform(viewTransform);
    auto levelInputData = m_levelView.GetObjectInputData(inputState);
    m_levelContainer.Update(levelInputData, elapsedTicks);
  }

  auto overlayInputData = m_overlayView.GetObjectInputData(inputState);
  m_overlayContainer.Update(overlayInputData, elapsedTicks);

  if( m_levelContainer.GetObjectContainer().IsComplete() )
    m_continueRunning = false;
}

auto play_screen::Render() const -> void
{
  m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  const auto& viewTransform = m_levelView.GetTransform();
  auto viewRect = m_levelView.GetViewRect();

  m_renderTarget->SetTransform(viewTransform);
  m_levelContainer.GetObjectContainer().Render(viewRect);

  const auto& overlayTransform = m_overlayView.GetTransform();
  auto overlayViewRect = m_levelView.GetViewRect();
  m_renderTarget->SetTransform(overlayTransform);
  m_overlayContainer.Render(overlayViewRect);
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

    if( m_playerHasThrusterOn )
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
    m_gameLevelDataLoader.SetPlayerPositionUpdate([this](float x, float y, bool thrusterOn) -> void
    {
      m_levelViewCentreX = x;
      m_levelViewCentreY = y;
      m_playerHasThrusterOn = thrusterOn;
    });
    
    m_gameLevelDataLoader.SetPlayerShot([this](float x, float y, float angle) -> void
    {
      m_levelContainer.GetObjectContainer().AppendActiveObject(bullet { x, y, angle });
      m_playerShot = true;
    });

    m_gameLevelDataLoader.SetPlayerDied([this](float x, float y) -> void
    {
      m_continueRunning = false;
    });

    m_gameLevelDataLoader.SetTargetActivated([this]() -> void
    {
      m_targetActivated = true;
    });

    m_gameLevelDataLoader.SetTimeout([this]() -> void
    {
      m_continueRunning = false;
    });

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
  m_levelContainer = m_gameLevelDataLoader.LoadLevel(m_renderTarget.get());
}

[[nodiscard]] auto play_screen::GetMenuDef() -> menu_def
{
  auto menuArea = render_target_area(m_renderTarget.get(), 0.3f, 0.3f);

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
