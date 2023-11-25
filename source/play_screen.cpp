#include "pch.h"
#include "play_screen.h"
#include "diagnostics.h"
#include "screen_view.h"
#include "global_state.h"
#include "render_target_area.h"
#include "diagnostics.h"
#include "sound_buffer_player.h"
#include "gamepad_trigger.h"
#include "diagnostics.h"

play_screen::play_screen() : m_levelContainer(std::make_unique<level_container>())
{
  if( !LoadFirstLevel() )
  {
    Quit();
  }

  auto renderTargetSize = render_target::get()->GetSize();

  auto menuArea = render_target_area { renderTargetSize, render_target_area::contraint_centred(0.4f, 0.4f) };
  m_menuController.OpenRoot(menuArea);

  auto playerPosition = m_levelContainer->PlayerPosition();
  m_startSequence = camera_sequence::camera_position { playerPosition.x, playerPosition.y, 0.1f };
  m_startSequence.AddMove( { playerPosition.x, playerPosition.y, m_playZoom }, performance_counter::CalculateTicks(3.0f) );

  m_playerShields.Attach(m_levelContainer->PlayerShields());
}

auto play_screen::Refresh(int64_t ticks) -> bool
{
  Update(ticks);
  Render();
  return m_continueRunning;
}

auto play_screen::Update(int64_t frameInterval) -> void
{
  diagnostics::addWindowData(main_window::data());

  auto startUpdateTime = performance_counter::QueryValue();

  switch( m_stage )
  {
    case stage::pre_play:
      PrePlay(frameInterval);
      break;
    case stage::playing:
      Playing(frameInterval);
      break;
    case stage::post_play:
      PostPlay(frameInterval);
      break;
  }

  auto overlayTransform = GetOverlayRenderTransform();

  if( Paused() )
  {
    m_menuController.Update(menu_control_data { user_input::keyboardReader(), user_input::gamepadReader() });

    switch( m_menuController.Selection() )
    {
      case play_menu_controller::selection::resume:
        Unpause();
        break;
      case play_menu_controller::selection::quit:
        Quit();
        break;
    }
  }

  auto endUpdateTime = performance_counter::QueryValue();

  diagnostics::setUpdateTime(endUpdateTime - startUpdateTime);
}

auto play_screen::Render() const -> void
{
  auto renderTargetSize = render_target::get()->GetSize();

  render_guard renderGuard { render_target::get() };

  auto startRenderTime = performance_counter::QueryValue();

  render_target::get()->Clear(D2D1::ColorF(0.4f, 0.4f, 0.4f, 1.0f));

  auto screenTransform = GetLevelRenderTransform();
  render_target::get()->SetTransform(screenTransform.Get());
  m_levelContainer->Render(screenTransform.GetViewRect(renderTargetSize));

  auto overlayRenderTransform = GetOverlayRenderTransform();
  render_target::get()->SetTransform(overlayRenderTransform.Get());

  if( m_stage == stage::playing )
  {
    m_levelRadar.Render(m_levelContainer->PlayerPosition(), m_levelContainer->Targets());
  }

  auto overlayViewRect = overlayRenderTransform.GetViewRect(renderTargetSize);

  renderer::render(m_playerShields);

  if( Paused() )
  {
    m_menuController.Render(overlayViewRect);
  }

  auto endRenderTime = performance_counter::QueryValue();

  diagnostics::setRenderTime(endRenderTime - startRenderTime);
  diagnostics::addTimingData(60);
  diagnostics::updateFrameData();
  renderer::renderDiagnostics();
  diagnostics::clear();
}

auto play_screen::PlaySoundEffects() const -> void
{
  if( m_levelContainer->PlayerHasThrusterOn() )
  {
    sound_data::get(sound_data::thrust).Play(true);
  }
  else
  {
    sound_data::get(sound_data::thrust).Stop();
  }

  if( m_levelContainer->UpdateEvents().playerShot )
  {
    sound_data::get(sound_data::shoot).Reset();
    sound_data::get(sound_data::shoot).Play(false);
  }

  if( m_levelContainer->UpdateEvents().targetActivated )
  {
    sound_data::get(sound_data::target_activated).Reset();
    sound_data::get(sound_data::target_activated).Play(false);
  }

  if( m_levelContainer->UpdateEvents().mineExploded )
  {
    sound_data::get(sound_data::mine_exploded).Reset();
    sound_data::get(sound_data::mine_exploded).Play(false);
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
    sound_data::get(sound_data::menu_theme).Play(true);
  }
}

auto play_screen::Playing(int64_t frameInterval) -> void
{
  if( PausePressed() )
  {
    if( Paused() )
    {
      Unpause();
    }
    else
    {
      Pause();
    }
  }

  if( !Paused() )
  {
    PlaySoundEffects();
    auto elapsedTicks = Paused() ? 0 : frameInterval;
    UpdateLevel(elapsedTicks);
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

    sound_data::get(sound_data::menu_theme).Stop();
    sound_data::get(sound_data::thrust).Stop();
  }
}

auto play_screen::PostPlay(int64_t frameInterval) -> void
{
  m_stageTicks += frameInterval;

  UpdateLevel(frameInterval);

  if( m_stageTicks > m_endSequence.GetTotalTicks() )
  {
    Quit();
  }
}

auto play_screen::UpdateLevel(int64_t elapsedTicks) -> void
{
  auto renderTransform = GetLevelRenderTransform();
  auto screenTransform = screen_transform { renderTransform.Get() };

  const auto levelInput = GetLevelInput(screenTransform);

  auto viewRect = screenTransform.GetViewRect(render_target::get()->GetSize());
  m_levelContainer->Update(levelInput, elapsedTicks, viewRect);
}

auto play_screen::GetLevelRenderTransform() const -> screen_transform
{
  auto renderTargetSize = render_target::get()->GetSize();
  auto cameraPosition = GetCameraPosition(renderTargetSize);
  auto cameraAngle = 0.0f;
  auto cameraTransform = play_camera_transform { cameraPosition.x, cameraPosition.y, cameraAngle, cameraPosition.scale, renderTargetSize };
  return { cameraTransform.Get() };
}

[[nodiscard]] auto play_screen::GetLevelInput(const screen_transform& transform) const -> level_input
{
  const auto& currentGamepadState = user_input::gamepadReader().CurrentState();

  if( currentGamepadState.Connected() )
  {
    gamepad_thumbstick leftThumbstick { currentGamepadState.ThumbLX(), currentGamepadState.ThumbLY(), 8000 };
    gamepad_thumbstick rightThumbstick { currentGamepadState.ThumbRX(), currentGamepadState.ThumbRY(), 5000 };
    gamepad_trigger leftTrigger { currentGamepadState.LeftTrigger() };
    gamepad_trigger rightTrigger { currentGamepadState.RightTrigger() };
    gamepad_buttons buttons { currentGamepadState.Buttons() };
    
    diagnostics::add(L"Left thumb X", leftThumbstick.X() );
    diagnostics::add(L"Left thumb Y", leftThumbstick.Y());
    diagnostics::add(L"Left thumb X ratio", leftThumbstick.XRatio());
    diagnostics::add(L"Left thumb Y ratio", leftThumbstick.YRatio());

    auto rotation = leftThumbstick.XRatio() * 10.0f;

    auto thrust = leftTrigger.Ratio();

    std::optional<float> shootAngle = rightTrigger.Value() > 0 ? std::optional<float>(m_levelContainer->PlayerAngle()) : std::nullopt;

    return { std::nullopt, rotation, thrust, shootAngle };
  }
  else
  {
    return { std::nullopt, std::nullopt, 0, std::nullopt };
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
  return user_input::gamepadReader().Pressed(XINPUT_GAMEPAD_BACK);
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

[[nodiscard]] auto play_screen::Paused() const -> bool
{
  return m_paused;
}

auto play_screen::Pause() -> void
{
  sound_data::get(sound_data::menu_theme).Stop();
  sound_data::get(sound_data::thrust).Stop();
  m_paused = true;
}

auto play_screen::Unpause() -> void
{
  m_paused = false;
  sound_data::get(sound_data::menu_theme).Play(true);
}

auto play_screen::Quit() -> void
{
  sound_data::get(sound_data::menu_theme).Stop();
  m_continueRunning = false;
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
