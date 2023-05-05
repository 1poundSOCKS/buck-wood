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

  m_continueRunning = LoadFirstLevel();

  auto menu = GetMenuDef().CreateMenu();

  menu.SetCallbackForHiddenFlag([this]() -> bool
  {
    return !m_paused;
  });

  text_box levelTimer({ renderTarget->GetSize(), 0.2f, 0.1f, render_target_area::vertical_bottom, render_target_area::horizontal_left });

  levelTimer.SetTextGetter([this]() -> std::wstring
  {
    auto value = static_cast<float>(m_levelContainer->TicksRemaining()) / static_cast<float>(performance_counter::QueryFrequency());
    return std::format(L"{:.1f}", value);
  });

  levelTimer.SetCallbackForHiddenFlag([this]() -> bool
  {
    return m_screenView.IsStarting();
  });

  m_overlayContainer.AppendOverlayObject(menu);
  m_overlayContainer.AppendOverlayObject(levelTimer);
  m_overlayContainer.AppendOverlayObject(mouse_cursor {});

  m_screenView.SetRenderTargetSize(renderTarget->GetSize());
}

auto play_screen::Update(const screen_input_state& inputState) -> void
{
  auto frameTicks = performance_counter::QueryFrequency() / framework::fps();

  if( PausePressed(inputState) && m_screenView.CanPauseScreen() )
  {
    m_paused = !m_paused;
  }

  m_screenView.Update(frameTicks);

  auto elapsedTicks = m_paused ? 0 : m_screenView.GetElapsedTicks(frameTicks);

  m_screenView.SetPlayerPosition(m_levelContainer->PlayerX(), m_levelContainer->PlayerY());

  m_levelTransform.Set(m_screenView.GetTransform());

  auto levelInputData = m_levelTransform.GetObjectInputData(inputState);
  m_levelContainer->Update(levelInputData, elapsedTicks);
  
  if( m_levelContainer->IsComplete() )
  {
    m_levelTimes.emplace_back(m_levelContainer->TicksRemaining());
  }

  if( m_levelContainer->HasTimedOut() || m_levelContainer->PlayerDied() || m_levelContainer->IsComplete() )
  {
    m_screenView.SwitchToEnding();
  }

  auto overlayInputData = m_overlayTransform.GetObjectInputData(inputState);
  m_overlayContainer.Update(overlayInputData, elapsedTicks);

  if( m_screenView.TimeToSwitch() )
  {
    m_screenView.Switch();
  }

  if( m_screenView.ScreenCanClose() )
  {
    m_continueRunning = false;
  }
}

auto play_screen::Render() const -> void
{
  m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  m_renderTarget->SetTransform(m_levelTransform.Get());
  m_levelContainer->GetObjectContainer().Render(m_levelTransform.GetViewRect(m_renderTarget.get()));

  m_renderTarget->SetTransform(m_overlayTransform.Get());
  m_overlayContainer.Render(m_overlayTransform.GetViewRect(m_renderTarget.get()));
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
