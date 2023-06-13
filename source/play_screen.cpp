#include "pch.h"
#include "play_screen.h"
#include "render.h"
#include "diagnostics.h"
#include "screen_view.h"
#include "global_state.h"
#include "render_target_area.h"

play_screen::play_screen() : m_levelContainer(std::make_unique<level_container>()), m_levelUpdateEvents(std::make_unique<level_container::update_events>())
{
  const auto& renderTarget = framework::renderTarget();
  auto renderTargetSize = renderTarget->GetSize();

  m_continueRunning = LoadFirstLevel();

  m_menu = GetMenuDef().CreateMenu();

  auto mainArea = render_target_area { renderTargetSize, render_target_area::contraint_centred(0.95f, 0.95f) }.GetRect();
  auto levelMapArea = render_target_area { mainArea, render_target_area::contraint_bottom_right(0.15f, 0.2f) }.GetRect();
  m_levelMap.SetRect(levelMapArea);

  m_frameTicks = performance_counter::QueryFrequency() / framework::fps();

  auto playerPosition = m_levelContainer->PlayerPosition();
  m_startSequence = camera_sequence::camera_position { playerPosition.x, playerPosition.y, 0.1f };
  m_startSequence.AddMove( { playerPosition.x, playerPosition.y, m_playZoom }, performance_counter::CalculateTicks(3.0f) );
}

auto play_screen::Refresh(const screen_input_state& inputState, int64_t ticks) -> void
{
  performance::UpdateFrameData(m_frameData);

  std::vector<std::wstring> diagnosticsData;
  diagnosticsData.reserve(50);

  // FormatDiagnostics(inputState, std::back_inserter(diagnosticsData));
  diagnosticsData.emplace_back(std::format(L"fps: {}", performance::GetFPS(m_frameData)));
  FormatDiagnostics(std::back_inserter(diagnosticsData));

  Update(inputState, ticks);

  {
    render_guard renderGuard { framework::renderTarget() };
    Render();
    framework::renderDiagnostics(diagnosticsData);
  }
  
  framework::present();
  PostPresent();
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

  m_cursor.Update(overlayInputData);

  if( m_paused )
  {
    m_menu.Update(overlayInputData);
  }
}

auto play_screen::Render() const -> void
{
  const auto& renderTarget = framework::renderTarget();
  auto renderTargetSize = renderTarget->GetSize();

  renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  auto screenTransform = GetLevelRenderTransform();
  renderTarget->SetTransform(screenTransform.Get());
  m_levelContainer->Render(screenTransform.GetViewRect(renderTargetSize));

  auto overlayRenderTransform = GetOverlayRenderTransform();
  renderTarget->SetTransform(overlayRenderTransform.Get());

  auto overlayViewRect = overlayRenderTransform.GetViewRect(renderTargetSize);

  if( m_paused )
  {
    m_menu.Render(overlayViewRect);
  }

  m_levelMap.Render(m_levelContainer->PlayerPosition(), m_levelContainer->Targets(), overlayViewRect);

  m_cursor.Render(overlayViewRect);
}

auto play_screen::PostPresent() const -> void
{
  const auto soundBuffers = global_sound_buffer_selector { sound_data::soundBuffers() };

  if( m_stage != stage::playing || m_paused || !m_continueRunning )
  {
    StopSoundBufferPlay(soundBuffers[sound_buffer_name::menu_theme]);
    StopSoundBufferPlay(soundBuffers[sound_buffer_name::thrust]);
  }
  else
  {
    PlaySoundBuffer(soundBuffers[sound_buffer_name::menu_theme], true);

    if( m_levelContainer->PlayerHasThrusterOn() )
      PlaySoundBuffer(soundBuffers[sound_buffer_name::thrust], true);
    else
      StopSoundBufferPlay(soundBuffers[sound_buffer_name::thrust]);

    if( m_levelUpdateEvents->playerShot )
    {
      ResetSoundBuffer(soundBuffers[sound_buffer_name::shoot]);
      PlaySoundBuffer(soundBuffers[sound_buffer_name::shoot]);
    }

    if( m_levelUpdateEvents->targetActivated )
    {
      ResetSoundBuffer(soundBuffers[sound_buffer_name::target_activated]);
      PlaySoundBuffer(soundBuffers[sound_buffer_name::target_activated]);
    }

    if( m_levelUpdateEvents->mineExploded )
    {
      ResetSoundBuffer(soundBuffers[sound_buffer_name::mine_exploded]);
      PlaySoundBuffer(soundBuffers[sound_buffer_name::mine_exploded]);
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

  if( !m_paused )
  {
    m_levelUpdateEvents = UpdateLevel(inputState, elapsedTicks);
  }

  if( m_levelContainer->IsComplete() )
  {
    m_levelTimes.emplace_back(m_levelContainer->TicksRemaining());
  }

  if( m_levelContainer->HasFinished() )
  {
    m_stage = stage::post_play;
    auto playerPosition = m_levelContainer->PlayerPosition();
    m_endSequence = camera_sequence::camera_position { playerPosition.x, playerPosition.y, m_playZoom };
    m_endSequence.AddPause(performance_counter::CalculateTicks(2.0f));
    m_endSequence.AddMove( { playerPosition.x, playerPosition.y, 0.1f }, performance_counter::CalculateTicks(5.0f) );
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

auto play_screen::UpdateLevel(const screen_input_state& inputState, int64_t elapsedTicks) -> level_container::update_events_ptr
{
  auto renderTransform = GetLevelRenderTransform();
  auto screenTransform = screen_transform { renderTransform.Get() };
  auto objectInputData = screenTransform.GetObjectInputData(inputState);
  const auto& renderTarget = framework::renderTarget();
  auto viewRect = screenTransform.GetViewRect(renderTarget->GetSize());
  return m_levelContainer->Update(objectInputData, elapsedTicks, viewRect);
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

  auto playerPosition = m_levelContainer->PlayerPosition();

  switch( m_stage )
  {
    case stage::pre_play:
      return { playerPosition.x, playerPosition.y, m_startSequence.GetScale(m_stageTicks) };

    case stage::post_play:
      // return m_endSequence.GetPosition(m_stageTicks);
      return { playerPosition.x, playerPosition.y, m_endSequence.GetScale(m_stageTicks) };

    default:
      return { playerPosition.x, playerPosition.y, m_playZoom };
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
    m_levelContainer = m_gameLevelDataLoader.LoadLevel();
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
    m_levelContainer = m_gameLevelDataLoader.LoadLevel();
    return true;
  }
}

[[nodiscard]] auto play_screen::GetMenuDef() -> menu_def
{
  const auto& renderTarget = framework::renderTarget();

  auto menuArea = render_target_area(renderTarget->GetSize(), render_target_area::contraint_centred(0.3f, 0.3f));
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
