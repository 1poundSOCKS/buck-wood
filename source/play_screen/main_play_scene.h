#pragma once

#include "play_scene.h"
#include "renderers.h"
#include "audio_events.h"

class main_play_scene : public play_scene
{

public:

  main_play_scene(const level_container_ptr& levelContainer, play_events playEvents) : play_scene(levelContainer, playEvents)
  {
    m_playerShields.Attach(m_levelContainer->PlayerShields());
  }

  ~main_play_scene()
  {
    sound_data::get(sound_data::main_theme).Stop();
    sound_data::get(sound_data::thrust).Stop();
  }

  auto Begin() -> void override
  {
    // SetCameraZoom(1);
    // sound_data::get(sound_data::main_theme).Play(true);
    audio_events::StartGameplayTheme();
  }

  auto End() -> void override
  {
    // sound_data::get(sound_data::main_theme).Stop();
    // sound_data::get(sound_data::thrust).Stop();
    audio_events::StopPlayerThruster();
    audio_events::StopGameplayTheme();
  }

  auto Pause() -> void override
  {
    // sound_data::get(sound_data::main_theme).Stop();
    // sound_data::get(sound_data::thrust).Stop();
    audio_events::StopPlayerThruster();
    audio_events::StopGameplayTheme();
  }

  auto Resume() -> void override
  {
    // sound_data::get(sound_data::main_theme).Play(true);
    audio_events::StartGameplayTheme();
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
