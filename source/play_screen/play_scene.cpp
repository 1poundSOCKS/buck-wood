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

  auto levelInput = ticks ? GetLevelInput() : level_input { std::nullopt, std::nullopt, 0, std::nullopt };
  m_levelContainer->Update(levelInput, ticks, viewRect);

  return m_levelContainer->HasFinished() ? false : true;
}

auto play_scene::Render() const -> void
{
  auto playerPosition = m_levelContainer->PlayerPosition();
  camera_sequence::camera_position cameraPosition { playerPosition.x, playerPosition.y, m_cameraZoom };
  auto cameraTransform = play_camera_transform { cameraPosition.x, cameraPosition.y, 0, cameraPosition.scale, render_target::get()->GetSize() };
  auto screenTransform = screen_transform { cameraTransform.Get() };
  auto viewRect = screenTransform.GetViewRect(render_target::get()->GetSize());

  render_target::get()->Clear(D2D1::ColorF(0.4f, 0.4f, 0.4f, 1.0f));
  render_target::get()->SetTransform(screenTransform.Get());
  m_levelContainer->Render(viewRect);
}

[[nodiscard]] auto play_scene::GetLevelInput() const -> level_input
{
  if( gamepad_reader::connected() )
  {
    diagnostics::add(L"Left thumb X", gamepad_reader::thumb_lx());
    diagnostics::add(L"Left thumb Y", gamepad_reader::thumb_ly());
    diagnostics::add(L"Left trigger", gamepad_reader::left_trigger());

    auto rotation = gamepad_reader::thumb_lx() * 10.0f;
    auto thrust = gamepad_reader::left_trigger();
    std::optional<float> shootAngle = gamepad_reader::right_trigger() > 0 ? std::optional<float>(m_levelContainer->PlayerAngle()) : std::nullopt;

    return { std::nullopt, rotation, thrust, shootAngle };
  }
  else
  {
    return { std::nullopt, std::nullopt, 0, std::nullopt };
  }
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

auto play_scene::SetCameraZoom(float value) -> void
{
  m_cameraZoom = value;
}
