#include "pch.h"
#include "play_scene.h"
#include "game_clock.h"
#include "hud_target.h"
#include "renderers.h"
#include "audio_events.h"

play_scene::play_scene(const level_container_ptr& levelContainer, play_events playEvents) : m_levelContainer { levelContainer }, m_playEvents { playEvents }
{
}

play_scene::~play_scene()
{
}

auto play_scene::Begin() -> void
{
}

auto play_scene::End() -> void
{
}

auto play_scene::Pause() -> void
{
}

auto play_scene::Resume() -> void
{
}

auto play_scene::Update(__int64 ticks) -> bool
{
  PlaySoundEffects();
  
  m_playEvents.Reset();
  
  auto thumbLX = gamepad_reader::thumb_lx();
  auto thumbLY = -gamepad_reader::thumb_ly();

  std::optional<D2D1_POINT_2F> leftThumbstickPosition = thumbLX || thumbLY  ? std::optional<D2D1_POINT_2F> { D2D1_POINT_2F { thumbLX, thumbLY } } : std::nullopt;

  m_playerDestination = leftThumbstickPosition ? std::optional<D2D1_POINT_2F>(direct2d::ShiftPosition(m_levelContainer->PlayerPosition(), *leftThumbstickPosition)) : std::nullopt;
  
  m_levelContainer->SetPlayerDestination(m_playerDestination);

  auto cxRight = gamepad_reader::thumb_rx() * 200;
  auto cyRight = gamepad_reader::thumb_ry() * -200;

  m_targetOffset = direct2d::ShiftPosition(m_levelContainer->PlayerPosition(), cxRight, cyRight);

  m_targetPosition = direct2d::ShiftPosition(m_levelContainer->PlayerPosition(), m_targetOffset);
  m_levelContainer->SetTargetPosition(m_targetPosition);
  m_levelContainer->SetTargetDirection(direct2d::GetAngleBetweenPoints(m_levelContainer->PlayerPosition(), m_targetPosition));

  m_levelContainer->Update(game_clock::getInterval(ticks), GetRenderTargetView());

  return m_levelContainer->HasFinished() ? false : true;
}

auto play_scene::Render() const -> void
{
  render_target::get()->Clear(D2D1::ColorF(0, 0, 0, 1.0f));
  auto transform = RenderTransform();
  render_target::get()->SetTransform(transform);
  m_levelContainer->Render(GetRenderTargetView(transform));

  auto targettedObject = m_levelContainer->TargettedObject();

  if( targettedObject )
  {
    auto bounds = targettedObject->Bounds(D2D1::Matrix3x2F::Identity());
    hud_target hudTarget { bounds };
    renderer::render(hudTarget);
  }

  renderer::render(target_position { m_targetPosition });
  if( m_playerDestination ) renderer::render(player_destination { *m_playerDestination });
}

auto play_scene::RenderTransform() const -> D2D1::Matrix3x2F
{
  auto playerPosition = m_levelContainer->PlayerPosition();
  camera_sequence::camera_position cameraPosition { playerPosition.x, playerPosition.y, m_cameraZoom };
  auto cameraTransform = play_camera_transform { cameraPosition.x, cameraPosition.y, 0, cameraPosition.scale, render_target::get()->GetSize() };
  return cameraTransform.Get();
}

auto play_scene::LevelContainer() const -> level_container_ptr
{
  return m_levelContainer;
}

auto play_scene::PlaySoundEffects() const -> void
{
  if( m_levelContainer->PlayerHasThrusterOn() )
  {
    audio_events::StartPlayerThruster();
  }
  else
  {
    audio_events::StopPlayerThruster();
  }

  if( m_playEvents[play_events::event_type::shot] )
  {
    audio_events::PlayerShot();
  }

  if( m_playEvents[play_events::event_type::target_activated] )
  {
    audio_events::TargetActivated();
  }

  if( m_playEvents[play_events::event_type::explosion] )
  {
    audio_events::Explosion();
  }
}

auto play_scene::SetCameraZoom(float value) -> void
{
  m_cameraZoom = value;
}

auto play_scene::GetRenderTargetView() const -> D2D1_RECT_F
{
  return GetRenderTargetView(RenderTransform());
}

auto play_scene::GetRenderTargetView(D2D1::Matrix3x2F transform) -> D2D1_RECT_F
{
  auto renderTargetSize = render_target::get()->GetSize();
  auto renderTargetTopLeft  = D2D1_POINT_2F { 0, 0 };
  auto renderTargetBottomRight  = D2D1_POINT_2F { renderTargetSize.width - 1.0f, renderTargetSize.height - 1.0f };

  transform.Invert();
  auto topLeft = transform.TransformPoint(renderTargetTopLeft);
  auto bottomRight = transform.TransformPoint(renderTargetBottomRight);
  auto bottomLeft = transform.TransformPoint({renderTargetTopLeft.x, renderTargetTopLeft.y});
  auto topRight = transform.TransformPoint({renderTargetTopLeft.x, renderTargetTopLeft.y});

  auto [minX, maxX] = std::minmax({topLeft.x, bottomRight.x, bottomLeft.x, topRight.x});
  auto [minY, maxY] = std::minmax({topLeft.y, bottomRight.y, bottomLeft.y, topRight.y});

  return { minX, minY, maxX, maxY };
}
