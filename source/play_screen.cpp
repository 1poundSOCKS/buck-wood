#include "pch.h"
#include "play_screen.h"
#include "render.h"
#include "diagnostics.h"
#include "screen_view.h"
#include "global_state.h"
#include "render_target_area.h"
#include "diagnostics.h"
#include "sound_buffer_player.h"
#include "gamepad_trigger.h"

play_screen::play_screen() : m_levelContainer(std::make_unique<level_container>())
{
  const auto& renderTarget = framework::renderTarget();
  auto renderTargetSize = renderTarget->GetSize();

  m_continueRunning = LoadFirstLevel();

  m_menu = GetMenuDef().CreateMenu();
  m_menu.SelectFirstButton();

  auto playerPosition = m_levelContainer->PlayerPosition();
  m_startSequence = camera_sequence::camera_position { playerPosition.x, playerPosition.y, 0.1f };
  m_startSequence.AddMove( { playerPosition.x, playerPosition.y, m_playZoom }, performance_counter::CalculateTicks(3.0f) );

  m_playerShields.Attach(m_levelContainer->PlayerShields());
}

auto play_screen::Refresh(int64_t ticks) -> bool
{
  // framework::toggleFullscreenOnKeypress(DIK_F12);

  auto levelUpdateEvents = Update(ticks);

  Render();
  
  framework::present();
  
  PostPresent(levelUpdateEvents);

  return m_continueRunning;
}

auto play_screen::Update(int64_t frameInterval) -> level_container::update_events_ptr
{
  auto startUpdateTime = performance_counter::QueryValue();

  level_container::update_events_ptr levelUpdateEvents;
  
  switch( m_stage )
  {
    case stage::pre_play:
      PrePlay(frameInterval);
      break;
    case stage::playing:
      levelUpdateEvents = Playing(frameInterval);
      break;
    case stage::post_play:
      PostPlay(frameInterval);
      break;
  }

  auto overlayTransform = GetOverlayRenderTransform();

  m_cursor.Update(framework::screenInputState());

  if( m_paused )
  {
    menu_control_data menuControlData { framework::screenInputState() };
    m_menu.Update(menuControlData);
  }

  auto endUpdateTime = performance_counter::QueryValue();

  framework::setDiagnosticsUpdateTime(endUpdateTime - startUpdateTime);

  return levelUpdateEvents;
}

auto play_screen::Render() const -> void
{
  const auto& renderTarget = framework::renderTarget();
  auto renderTargetSize = renderTarget->GetSize();

  render_guard renderGuard { framework::renderTarget() };

  auto startRenderTime = performance_counter::QueryValue();

  renderTarget->Clear(D2D1::ColorF(0.4f, 0.4f, 0.4f, 1.0f));

  auto screenTransform = GetLevelRenderTransform();
  renderTarget->SetTransform(screenTransform.Get());
  m_levelContainer->Render(screenTransform.GetViewRect(renderTargetSize));

  auto overlayRenderTransform = GetOverlayRenderTransform();
  renderTarget->SetTransform(overlayRenderTransform.Get());

  if( m_stage == stage::playing )
  {
    m_levelRadar.Render(m_levelContainer->PlayerPosition(), m_levelContainer->Targets());
  }

  auto overlayViewRect = overlayRenderTransform.GetViewRect(renderTargetSize);

  renderer::render(m_playerShields);

  if( m_paused )
  {
    m_menu.Render(overlayViewRect);
  }

  auto endRenderTime = performance_counter::QueryValue();

  framework::setDiagnosticsRenderTime(endRenderTime - startRenderTime);

  framework::renderDiagnostics();
}

auto play_screen::PostPresent(const level_container::update_events_ptr& levelUpdateEvents) const -> void
{
  if( m_stage != stage::playing || m_paused || !m_continueRunning )
  {
    sound_data::get(sound_data::menu_theme).Stop();
    sound_data::get(sound_data::thrust).Stop();
  }
  else
  {
    sound_data::get(sound_data::menu_theme).Play(true);

    if( m_levelContainer->PlayerHasThrusterOn() )
    {
      sound_data::get(sound_data::thrust).Play(true);
    }
    else
    {
      sound_data::get(sound_data::thrust).Stop();
    }

    if( levelUpdateEvents.get() && levelUpdateEvents->playerShot )
    {
      sound_data::get(sound_data::shoot).Reset();
      sound_data::get(sound_data::shoot).Play(false);
    }

    if( levelUpdateEvents.get() && levelUpdateEvents->targetActivated )
    {
      sound_data::get(sound_data::target_activated).Reset();
      sound_data::get(sound_data::target_activated).Play(false);
    }

    if( levelUpdateEvents.get() && levelUpdateEvents->mineExploded )
    {
      sound_data::get(sound_data::mine_exploded).Reset();
      sound_data::get(sound_data::mine_exploded).Play(false);
    }
  }
}

auto play_screen::PrePlay(int64_t frameInterval) -> void
{
  m_stageTicks += frameInterval;

  UpdateLevel(0);

  if( m_stageTicks > m_startSequence.GetTotalTicks() )
  {
    m_stage = stage::playing;
    m_stageTicks = 0;
  }
}

auto play_screen::Playing(int64_t frameInterval) -> level_container::update_events_ptr
{
  level_container::update_events_ptr levelUpdateEvents;

  if( PausePressed() )
  {
    m_paused = !m_paused;
  }

  auto elapsedTicks = m_paused ? 0 : frameInterval;

  if( !m_paused )
  {
    levelUpdateEvents = UpdateLevel(elapsedTicks);
  }

  if( m_levelContainer->IsComplete() )
  {
    m_levelTimes.emplace_back(m_levelContainer->TicksRemaining());
  }

  if( m_levelContainer->HasFinished() )
  {
    m_stage = stage::post_play;
    auto playerPosition = m_levelContainer->PlayerPosition();
    auto levelCentre = m_levelContainer->Centre();
    m_endSequence = camera_sequence::camera_position { playerPosition.x, playerPosition.y, m_playZoom };
    m_endSequence.AddPause(performance_counter::CalculateTicks(2.0f));
    m_endSequence.AddMove( { levelCentre.x, levelCentre.y, 0.1f }, performance_counter::CalculateTicks(5.0f) );
    m_endSequence.AddPause(performance_counter::CalculateTicks(3.0f));
    m_stageTicks = 0;
  }

  return levelUpdateEvents;
}

auto play_screen::PostPlay(int64_t frameInterval) -> void
{
  m_stageTicks += frameInterval;

  UpdateLevel(frameInterval);

  if( m_stageTicks > m_endSequence.GetTotalTicks() )
  {
    m_continueRunning = false;
  }
}

auto play_screen::UpdateLevel(int64_t elapsedTicks) -> level_container::update_events_ptr
{
  auto renderTransform = GetLevelRenderTransform();
  auto screenTransform = screen_transform { renderTransform.Get() };

  const auto levelInput = GetLevelInput(framework::screenInputState(), screenTransform);

  const auto& renderTarget = framework::renderTarget();
  auto viewRect = screenTransform.GetViewRect(renderTarget->GetSize());
  return m_levelContainer->Update(levelInput, elapsedTicks, viewRect);
}

auto play_screen::GetLevelRenderTransform() const -> screen_transform
{
  const auto& renderTarget = framework::renderTarget();
  auto renderTargetSize = renderTarget->GetSize();
  auto cameraPosition = GetCameraPosition(renderTargetSize);
  auto cameraAngle = 0.0f;
  auto cameraTransform = play_camera_transform { cameraPosition.x, cameraPosition.y, cameraAngle, cameraPosition.scale, renderTargetSize };
  return { cameraTransform.Get() };
}

[[nodiscard]] auto play_screen::GetLevelInput(const screen_input_state& input, const screen_transform& transform) const -> level_input
{
  const auto& currentGamepadState = input.gamepadReader.CurrentState();

  if( currentGamepadState.Connected() )
  {
    gamepad_thumbstick leftThumbstick { currentGamepadState.ThumbLX(), currentGamepadState.ThumbLY(), 8000 };
    gamepad_thumbstick rightThumbstick { currentGamepadState.ThumbRX(), currentGamepadState.ThumbRY(), 5000 };
    gamepad_trigger leftTrigger { currentGamepadState.LeftTrigger() };
    gamepad_trigger rightTrigger { currentGamepadState.RightTrigger() };
    gamepad_buttons buttons { currentGamepadState.Buttons() };
    
    framework::addDiagnostics(L"Left thumb X", leftThumbstick.X() );
    framework::addDiagnostics(L"Left thumb Y", leftThumbstick.Y());
    framework::addDiagnostics(L"Left thumb X ratio", leftThumbstick.XRatio());
    framework::addDiagnostics(L"Left thumb Y ratio", leftThumbstick.YRatio());

    auto rotation = leftThumbstick.XRatio() * 10.0f;

    auto thrust = leftTrigger.Ratio();

    std::optional<float> shootAngle = rightTrigger.Value() > 0 ? std::optional<float>(m_levelContainer->PlayerAngle()) : std::nullopt;

    return { std::nullopt, rotation, thrust, shootAngle };
  }
  else
  {
    auto mousePosition = transform.GetScreenPosition({ input.renderTargetMouseData.x, input.renderTargetMouseData.y });
    auto previousMousePosition = transform.GetScreenPosition({ input.previousRenderTargetMouseData.x, input.previousRenderTargetMouseData.y });

    auto playerPosition = m_levelContainer->PlayerPosition();
    auto playerAngle = playerPosition.AngleTo(mousePosition);

    auto thrust = input.windowData.mouse.rightButtonDown ? 1.0f : 0;
    auto shootAngle = input.windowData.mouse.leftButtonDown ? std::optional<float>(playerAngle) : std::nullopt;

    return { playerAngle, std::nullopt, thrust, shootAngle };
  }
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
      return { m_endSequence.GetPosition(m_stageTicks).x, m_endSequence.GetPosition(m_stageTicks).y, m_endSequence.GetScale(m_stageTicks) };

    default:
      return { playerPosition.x, playerPosition.y, m_playZoom };
  }
}

[[nodiscard]] auto play_screen::PausePressed() -> bool
{
  menu_control_data menuControlData { framework::screenInputState() };
  return menuControlData.Back();
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
