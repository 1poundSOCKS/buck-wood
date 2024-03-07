#include "pch.h"
#include "play_scene.h"
#include "game_clock.h"
#include "hud_target.h"
#include "renderers.h"
#include "audio_events.h"
#include "line_to_target.h"

play_scene::play_scene(std::shared_ptr<level_container> levelContainer, std::shared_ptr<play_state> playState) : 
  m_levelContainer { levelContainer }, m_playState { playState }, m_levelTitle { levelContainer->Index() }
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
  m_playState->Events().Reset();
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

#ifdef RENDER_TARGET_HUD
  if( targettedObject )
  {
    renderer::render(line_to_target { *m_levelContainer->PlayerPosition(), targettedObject->Position() });
    
    auto bounds = targettedObject->Bounds(D2D1::Matrix3x2F::Identity());
    hud_target hudTarget { bounds };
    renderer::render(hudTarget);
  }
#endif

  if( m_renderLevelTitle )
  {
    render_target::get()->SetTransform(D2D1::Matrix3x2F::Identity());
    renderer::render(m_levelTitle);
  }
}

auto play_scene::RenderTransform() const -> D2D1::Matrix3x2F
{
  auto cameraPosition = CameraPosition();
  auto cameraTransform = play_camera_transform { cameraPosition.x, cameraPosition.y, 0, cameraPosition.scale, render_target::get()->GetSize() };
  return cameraTransform.Get();
}

auto play_scene::CameraPosition() const -> camera_sequence::camera_position
{
  auto playerPosition = m_levelContainer->PlayerState().Position();
  auto viewHeight = render_target::get()->GetSize().height / m_cameraZoom;

  switch( m_levelContainer->Type() )
  {
    case level_container::level_type::vertical_scroller:
      return camera_sequence::camera_position { 0, playerPosition.y - viewHeight / 3, m_cameraZoom };
    case level_container::level_type::arena:
      return camera_sequence::camera_position { 0, 0, m_cameraZoom };
    default:
      return camera_sequence::camera_position { playerPosition.x, playerPosition.y, m_cameraZoom };
  }
}

auto play_scene::LevelContainer() const -> std::shared_ptr<level_container>
{
  return m_levelContainer;
}

auto play_scene::PlaySoundEffects() const -> void
{
  if( m_levelContainer->PlayerState().ThrusterOn() )
  {
    audio_events::StartPlayerThruster();
  }
  else
  {
    audio_events::StopPlayerThruster();
  }

  if( m_playState->Events().Get(play_events::event_type::shot) )
  {
    audio_events::PlayerShot();
  }

  if( m_playState->Events().Get(play_events::event_type::target_activated) )
  {
    audio_events::TargetActivated();
  }

  if( m_playState->Events().Get(play_events::event_type::explosion) )
  {
    audio_events::Explosion();
  }
}

auto play_scene::SetCameraZoom(float value) -> void
{
  m_cameraZoom = value;
}

[[nodiscard]] auto play_scene::GetPlayCameraZoom() const -> float
{
  auto levelSize = m_levelContainer->LevelSize();
  auto renderTargetSize = render_target::get()->GetSize();

  switch( m_levelContainer->Type() )
  {
    case level_container::level_type::vertical_scroller:
      return renderTargetSize.width / levelSize.width * 0.8f;
    case level_container::level_type::arena:
      return renderTargetSize.height / levelSize.height * 0.9f;
    default:
      return 1.0f;
  }
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
