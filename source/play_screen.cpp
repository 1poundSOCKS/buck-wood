#include "pch.h"
#include "play_screen.h"
#include "render.h"
#include "diagnostics.h"
#include "screen_view.h"
#include "global_state.h"
#include "render_target_area.h"
#include "text_box.h"

play_screen::play_screen() : m_levelContainer(std::make_unique<level_container>())
{
  const auto& renderTarget = framework::renderTarget();

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
    return m_stage == stage::pre_play;
  });

  m_overlayContainer += menu;
  m_overlayContainer += levelTimer;
  m_overlayContainer += mouse_cursor {};

  m_frameTicks = performance_counter::QueryFrequency() / framework::fps();

  auto renderTargetSize = renderTarget->GetSize();
  m_startSequence = camera_sequence::camera_position { m_levelContainer->PlayerX(), m_levelContainer->PlayerY(), 5.0f };
  m_startSequence.AddMove( { m_levelContainer->PlayerX(), m_levelContainer->PlayerY(), m_playZoom }, performance_counter::CalculateTicks(3.0f) );
}

auto play_screen::Update(const screen_input_state& inputState, int64_t frameInterval) -> void
{
  switch( m_stage )
  {
    case stage::pre_play:
      PrePlay(inputState, frameInterval);
      break;
    case stage::playing:
      Playing(inputState, frameInterval);
      break;
    case stage::post_play:
      PostPlay(inputState, frameInterval);
      break;
  }

  auto overlayTransform = GetOverlayRenderTransform();
  auto overlayInputData = overlayTransform.GetObjectInputData(inputState);
  m_overlayContainer.Update(overlayInputData, frameInterval);
}

auto play_screen::Render() const -> void
{
  const auto& renderTarget = framework::renderTarget();

  auto renderTargetSize = renderTarget->GetSize();

  renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  auto levelRenderTransform = GetLevelRenderTransform();
  auto screenTransform = screen_transform { levelRenderTransform.Get() };
  renderTarget->SetTransform(levelRenderTransform.Get());
  m_levelContainer->Render(renderTarget.get(), screenTransform.GetViewRect(renderTargetSize));

  auto overlayRenderTransform = GetOverlayRenderTransform();
  renderTarget->SetTransform(overlayRenderTransform.Get());
  m_overlayContainer.Render(overlayRenderTransform.GetViewRect(renderTargetSize));
}

auto play_screen::PostPresent() const -> void
{
  const auto soundBuffers = global_sound_buffer_selector { sound_data::soundBuffers() };

  if( m_stage != stage::playing || m_paused || !m_continueRunning )
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

auto play_screen::PrePlay(const screen_input_state& inputState, int64_t frameInterval) -> void
{
  m_stageTicks += frameInterval;

  UpdateLevel(inputState, 0);

  if( m_stageTicks > m_startSequence.GetTotalTicks() )
  {
    m_stage = stage::playing;
    m_stageTicks = 0;
  }
}

auto play_screen::Playing(const screen_input_state& inputState, int64_t frameInterval) -> void
{
  if( PausePressed(inputState) )
  {
    m_paused = !m_paused;
  }

  auto elapsedTicks = m_paused ? 0 : frameInterval;

  UpdateLevel(inputState, elapsedTicks);

  if( m_levelContainer->IsComplete() )
  {
    m_levelTimes.emplace_back(m_levelContainer->TicksRemaining());
  }

  if( m_levelContainer->HasFinished() )
  {
    m_stage = stage::post_play;
    m_endSequence = camera_sequence::camera_position { m_levelContainer->PlayerX(), m_levelContainer->PlayerY(), m_playZoom };
    m_endSequence.AddPause(performance_counter::CalculateTicks(2.0f));
    m_endSequence.AddMove( { m_levelContainer->PlayerX(), m_levelContainer->PlayerY(), 0.1f }, performance_counter::CalculateTicks(5.0f) );
    m_endSequence.AddPause(performance_counter::CalculateTicks(3.0f));
    m_stageTicks = 0;
  }
}

auto play_screen::PostPlay(const screen_input_state& inputState, int64_t frameInterval) -> void
{
  m_stageTicks += frameInterval;

  UpdateLevel(inputState, frameInterval);

  if( m_stageTicks > m_endSequence.GetTotalTicks() )
  {
    m_continueRunning = false;
  }
}

auto play_screen::UpdateLevel(const screen_input_state& inputState, int64_t elapsedTicks) -> void
{
  auto renderTransform = GetLevelRenderTransform();
  auto screenTransform = screen_transform { renderTransform.Get() };
  auto objectInputData = screenTransform.GetObjectInputData(inputState);
  const auto& renderTarget = framework::renderTarget();
  auto viewRect = screenTransform.GetViewRect(renderTarget->GetSize());
  m_levelContainer->Update(objectInputData, elapsedTicks, viewRect);
}

auto play_screen::GetLevelRenderTransform() const -> screen_transform
{
  const auto& renderTarget = framework::renderTarget();
  auto renderTargetSize = renderTarget->GetSize();
  auto cameraPosition = GetCameraPosition(renderTargetSize);
  auto cameraTransform = play_camera_transform { cameraPosition.x, cameraPosition.y, cameraPosition.scale, renderTargetSize };
  return { cameraTransform.Get() };
}

auto play_screen::GetOverlayRenderTransform() const -> screen_transform
{
  return {};
}

auto play_screen::GetCameraPosition(D2D1_SIZE_F renderTargetSize) const -> camera_sequence::camera_position
{
  camera_sequence::camera_position cameraPosition = { 0, 0, 1.0f };

  switch( m_stage )
  {
    case stage::pre_play:
      return m_startSequence.GetPosition(m_stageTicks);

    case stage::post_play:
      return m_endSequence.GetPosition(m_stageTicks);

    default:
      return { m_levelContainer->PlayerX(), m_levelContainer->PlayerY(), m_playZoom };
  }
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
  const auto& renderTarget = framework::renderTarget();

  m_levelContainer = m_gameLevelDataLoader.LoadLevel(renderTarget.get());
}

[[nodiscard]] auto play_screen::GetMenuDef() -> menu_def
{
  const auto& renderTarget = framework::renderTarget();

  auto menuArea = render_target_area(renderTarget->GetSize(), 0.3f, 0.3f);

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
