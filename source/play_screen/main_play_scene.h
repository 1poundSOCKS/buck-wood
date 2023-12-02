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
    sound_data::get(sound_data::menu_theme).Play(true);
  }

  auto Refresh(__int64 ticks) -> bool override
  {
    SetCameraZoom(1);
    play_scene::Refresh(ticks);

    render_guard renderGuard { render_target::get() };
    render_target::get()->SetTransform(D2D1::Matrix3x2F::Identity());
    renderer::render(m_playerShields);
    level_radar levelRadar { m_levelContainer->PlayerPosition() };
    renderer::render(levelRadar, m_levelContainer->Targets());

    return m_levelContainer->HasFinished() ? false : true;
  }

  auto End() -> void override
  {
    sound_data::get(sound_data::menu_theme).Stop();
    sound_data::get(sound_data::thrust).Stop();
  }

private:

  player_shields m_playerShields;

};
