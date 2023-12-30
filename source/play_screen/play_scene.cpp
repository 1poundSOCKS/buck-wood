#include "pch.h"
#include "play_scene.h"

play_scene::play_scene(const level_container_ptr& levelContainer) : m_levelContainer { levelContainer }
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

  auto playerPosition = m_levelContainer->PlayerPosition();
  camera_sequence::camera_position cameraPosition { playerPosition.x, playerPosition.y, 1 };
  auto cameraTransform = play_camera_transform { cameraPosition.x, cameraPosition.y, 0, cameraPosition.scale, render_target::get()->GetSize() };
  auto screenTransform = screen_transform { cameraTransform.Get() };
  auto viewRect = screenTransform.GetViewRect(render_target::get()->GetSize());

  m_levelUpdateEvents = m_levelContainer->Update(ticks, viewRect);

  return m_levelContainer->HasFinished() ? false : true;
}

auto play_scene::Render() const -> void
{
  auto screenTransform = screen_transform { RenderTransform() };
  auto viewRect = screenTransform.GetViewRect(render_target::get()->GetSize());

  render_target::get()->Clear(D2D1::ColorF(0, 0, 0, 1.0f));
  render_target::get()->SetTransform(screenTransform.Get());
  m_levelContainer->Render(viewRect);
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
    sound_data::get(sound_data::thrust).Play(true);
  }
  else
  {
    sound_data::get(sound_data::thrust).Stop();
  }

  if( m_levelUpdateEvents.playerShot )
  {
    sound_data::get(sound_data::shoot).Reset();
    sound_data::get(sound_data::shoot).Play(false);
  }

  if( m_levelUpdateEvents.targetActivated )
  {
    sound_data::get(sound_data::target_activated).Reset();
    sound_data::get(sound_data::target_activated).Play(false);
  }

  if( m_levelUpdateEvents.mineExploded )
  {
    sound_data::get(sound_data::mine_exploded).Reset();
    sound_data::get(sound_data::mine_exploded).Play(false);
  }
}

auto play_scene::SetCameraZoom(float value) -> void
{
  m_cameraZoom = value;
}
