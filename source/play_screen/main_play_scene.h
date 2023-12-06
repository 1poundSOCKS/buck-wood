#pragma once

#include "play_scene.h"

class main_play_scene : public play_scene
{

public:

  main_play_scene(const level_container_ptr& levelContainer) : play_scene { levelContainer }
  {
    m_playerShields.Attach(m_levelContainer->PlayerShields());
  }

  ~main_play_scene()
  {
    sound_data::get(sound_data::menu_theme).Stop();
    sound_data::get(sound_data::thrust).Stop();
  }

  auto Begin() -> void override
  {
    SetCameraZoom(1);
    sound_data::get(sound_data::menu_theme).Play(true);
  }

  auto End() -> void override
  {
    sound_data::get(sound_data::menu_theme).Stop();
    sound_data::get(sound_data::thrust).Stop();
  }

  auto Pause() -> void override
  {
    sound_data::get(sound_data::menu_theme).Stop();
    sound_data::get(sound_data::thrust).Stop();
  }

  auto Resume() -> void override
  {
    sound_data::get(sound_data::menu_theme).Play(true);
  }

  auto Update(int64_t ticks) -> bool override
  {
    return play_scene::Update(ticks);
  }

  auto Render() const -> void override
  {
    play_scene::Render();
    render_target::get()->SetTransform(D2D1::Matrix3x2F::Identity());
    renderer::render(m_playerShields);
    renderer::render(level_radar { m_levelContainer->PlayerPosition() }, m_levelContainer->Targets());
  }

private:

  player_shields m_playerShields;

};
