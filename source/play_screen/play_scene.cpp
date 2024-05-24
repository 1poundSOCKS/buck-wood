#include "pch.h"
#include "play_scene.h"
#include "game_clock.h"
#include "hud_target.h"
#include "audio_events.h"
#include "line_to_target.h"
#include "game_settings.h"
#include "game_state.h"
#include "energy_bar_rect.h"

play_scene::play_scene(std::shared_ptr<play_state> playState) : 
  m_playState { playState }, m_levelTitle { game_state::level_index() }
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
  m_paused = true;
}

auto play_scene::Resume() -> void
{
  m_paused = false;
}

auto play_scene::Update(__int64 ticks) -> bool
{
  PlaySoundEffects();
  play_events::reset();
  m_playState->Update(game_clock::getInterval(ticks), GetRenderTargetView());
  m_renderTransform = RenderTransform();
  return m_playState->Status() == play_state::status::running;
}

auto play_scene::Render() const -> void
{
  render_target::get()->Clear(D2D1::ColorF(0.15f, 0.15f, 0.15f, 1.0f));
  render_target::get()->SetTransform(m_renderTransform);

  RenderLevelContainer();

  auto targettedObject = m_playState->LevelContainer().TargettedObject();

  if( targettedObject )
  {
    auto playerPosition = m_playState->LevelContainer().PlayerPosition();

#if 0
    renderer::render(line_to_target { playerPosition, targettedObject->Position() });

    auto bounds = targettedObject->Bounds(D2D1::Matrix3x2F::Identity());
    hud_target hudTarget { bounds };
    renderer::render(hudTarget);
#endif
  }

  render_target::get()->SetTransform(D2D1::Matrix3x2F::Identity());

  if( game_settings::showDiagnostics() )
  {
    RenderGeometryBoundaries();
  }

  if( !m_paused && m_renderLevelTitle )
  {
    renderer::render(m_levelTitle);
  }
}

[[nodiscard]] auto play_scene::RenderTransform() const noexcept -> D2D1::Matrix3x2F
{
  auto cameraPosition = CameraPosition();
  auto cameraTransform = play_camera_transform { cameraPosition.x, cameraPosition.y, 0, cameraPosition.scale, render_target::get()->GetSize() };
  return cameraTransform.Get();
}

auto play_scene::CameraPosition() const -> camera_sequence::camera_position
{
  auto playerPosition = m_playState->LevelContainer().PlayerPosition();
  return camera_sequence::camera_position { playerPosition.x, playerPosition.y, m_cameraZoom };
}

auto play_scene::RenderLevelContainer() const -> void
{
  auto renderStart = performance_counter::QueryValue();

  renderer::render(m_playState->LevelContainer());

  RenderEnergyBars();

  auto renderEnd = performance_counter::QueryValue();

  diagnostics::addTime(L"render", renderEnd - renderStart, game_settings::swapChainRefreshRate());
}

auto play_scene::PlaySoundEffects() const -> void
{
  if( !m_playState->LevelContainer().PlayerDestroyed() && m_playState->LevelContainer().PlayerThrusterOn() )
  {
    audio_events::StartPlayerThruster();
  }
  else
  {
    audio_events::StopPlayerThruster();
  }

  if( play_events::get(play_events::event_type::shot) )
  {
    audio_events::PlayerShot();
  }

  if( play_events::get(play_events::event_type::explosion) )
  {
    audio_events::Explosion();
  }

  if( play_events::get(play_events::counter_type::power_ups_collected) )
  {
    audio_events::PowerUpCollected();
  }
}

auto play_scene::RenderEnergyBars() const -> void
{
  m_playState->LevelContainer().EnumerateEnemyCollisionObjects([this](const auto& object)
  {
    if( object.Object().HoldsAlternative<enemy_type_1>() || object.Object().HoldsAlternative<enemy_type_2>() || object.Object().HoldsAlternative<enemy_type_3>() )
    {
      auto geometryBounds = object.Bounds();
      auto energyBarRect = energy_bar_rect { geometryBounds };
      auto health = object.Object().Health();
      auto energyBar = energy_bar { energyBarRect.Get(), health };
      renderer::render(energyBar);
    }
  });
}

auto play_scene::RenderGeometryBoundaries() const -> void
{
#if 0
  m_playState->LevelContainer().EnumerateInteractiveObjects([this](const auto& object)
  {
    renderer::render(ObjectRenderRect(object));
  });
#endif
}

auto play_scene::SetCameraZoom(float value) -> void
{
  m_cameraZoom = value;
}

[[nodiscard]] auto play_scene::GetPlayCameraZoom() const -> float
{
  auto levelSize = m_playState->LevelContainer().LevelSize();
  auto renderTargetSize = render_target::get()->GetSize();
  return renderTargetSize.height / 1800;
}

auto play_scene::GetRenderTargetView() const -> D2D1_RECT_F
{
  return GetRenderTargetView(m_renderTransform);
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
