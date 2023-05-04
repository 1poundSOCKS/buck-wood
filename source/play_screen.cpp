#include "pch.h"
#include "play_screen.h"
#include "render.h"
#include "diagnostics.h"
#include "screen_view.h"
#include "global_state.h"
#include "render_target_area.h"
#include "text_box.h"
#include "level_transform_transition.h"

play_screen::play_screen() : m_levelContainer(std::make_unique<level_container>())
{
}

auto play_screen::Initialize(ID2D1RenderTarget* renderTarget) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();

  m_levelContainer->GetObjectContainer().Initialize(renderTarget);
  m_overlayContainer.Initialize(renderTarget);

  m_levelView.Initialize(renderTarget);
  m_overlayView.Initialize(renderTarget);

  m_continueRunning = LoadFirstLevel();

  auto menu = GetMenuDef().CreateMenu();

  menu.SetCallbackForHiddenFlag([this]() -> bool
  {
    return !m_paused || m_view == view_ending;
  });

  text_box levelTimer({ renderTarget->GetSize(), 0.2f, 0.1f, render_target_area::vertical_bottom, render_target_area::horizontal_left });

  levelTimer.SetTextGetter([this]() -> std::wstring
  {
    auto value = static_cast<float>(m_levelContainer->TicksRemaining()) / static_cast<float>(performance_counter::QueryFrequency());
    return std::format(L"{:.1f}", value);
  });

  levelTimer.SetCallbackForHiddenFlag([this]() -> bool
  {
    return m_view == view_starting;
  });

  m_overlayContainer.AppendOverlayObject(menu);
  m_overlayContainer.AppendOverlayObject(levelTimer);
  m_overlayContainer.AppendOverlayObject(mouse_cursor {});

  m_view = view_starting;
  m_totalStartingTicks = performance_counter::QueryFrequency() * 3;  
}

auto play_screen::Update(const screen_input_state& inputState) -> void
{
  auto frameTicks = performance_counter::QueryFrequency() / framework::fps();

  if( PausePressed(inputState) && m_view != view_ending )
  {
    m_paused = !m_paused;
  }

  if( m_view == view_starting && !m_paused )
  {
    m_startingTicks += frameTicks;
  }

  if( m_view == view_ending )
  {
    m_endingTicks += frameTicks;
  }

  auto elapsedTicks = ( m_view == view_starting || m_view == view_ending || m_paused ) ? 0 : frameTicks;

  auto viewTransform = GetViewTransform();
  m_levelView.SetTransform(viewTransform);

  auto levelInputData = m_levelView.GetObjectInputData(inputState);
  m_levelContainer->Update(levelInputData, elapsedTicks);
  
  auto overlayInputData = m_overlayView.GetObjectInputData(inputState);
  m_overlayContainer.Update(overlayInputData, elapsedTicks);

  if( m_levelContainer->HasTimedOut() || m_levelContainer->PlayerDied() || m_levelContainer->IsComplete() )
  {
    m_view = view_ending;
    m_totalEndingTicks = performance_counter::QueryFrequency() * 5;
  }

  if( m_levelContainer->IsComplete() )
  {
    m_levelTimes.emplace_back(m_levelContainer->TicksRemaining());
  }

  switch( m_view )
  {
    case view_starting:
      if( m_startingTicks >= m_totalStartingTicks )
      {
        m_view = view_playing;
      }
      break;

    case view_ending:
      if( m_endingTicks >= m_totalEndingTicks )
      {
        m_continueRunning = false;
      }
      break;
  }
}

auto play_screen::Render() const -> void
{
  m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  const auto& viewTransform = m_levelView.GetTransform();
  auto viewRect = m_levelView.GetViewRect();

  m_renderTarget->SetTransform(viewTransform);
  m_levelContainer->GetObjectContainer().Render(viewRect);

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

    if( m_levelContainer->PlayerHasThrusterOn() )
      PlaySoundBuffer(soundBuffers[thrust], true);
    else
      StopSoundBufferPlay(soundBuffers[thrust]);

    if( m_levelContainer->PlayerShot() )
    {
      ResetSoundBuffer(soundBuffers[shoot]);
      PlaySoundBuffer(soundBuffers[shoot]);
    }

    if( m_levelContainer->TargetActivated() )
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
  m_levelContainer = m_gameLevelDataLoader.LoadLevel(m_renderTarget.get());
}

[[nodiscard]] auto play_screen::GetMenuDef() -> menu_def
{
  auto menuArea = render_target_area(m_renderTarget->GetSize(), 0.3f, 0.3f);

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

[[nodiscard]] auto play_screen::GetViewTransform() const -> D2D1::Matrix3x2F
{
  auto renderTargetSize = m_renderTarget->GetSize();

  if( m_view == view_starting )
  {
    level_transform_transition levelTransformTransiton(0.0f, 0.0f, 0.3f, m_levelContainer->PlayerX(), m_levelContainer->PlayerY(), 1.4f);
    return levelTransformTransiton.Get(renderTargetSize.width, renderTargetSize.height, m_totalStartingTicks, m_startingTicks);
  }
  else if( m_view == view_ending )
  {
    return CreateGameLevelTransform(0.0f, 0.0f, 0.3f, renderTargetSize.width, renderTargetSize.height);
  }
  else
  {
    return CreateGameLevelTransform(m_levelContainer->PlayerX(), m_levelContainer->PlayerY(), 1.4f, renderTargetSize.width, renderTargetSize.height);
  }
}
