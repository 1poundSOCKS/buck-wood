#pragma once

#include "play_scene.h"
#include "renderers.h"
#include "audio_events.h"

class main_play_scene : public play_scene
{

public:

  main_play_scene(std::shared_ptr<level_container> levelContainer, play_events playEvents) : 
    play_scene(levelContainer, playEvents), m_playerShields { levelContainer->PlayerState().Shields() }
  {
  }

  ~main_play_scene()
  {
    audio_events::StopPlayerThruster();
    audio_events::StopGameplayTheme();
  }

  auto Begin() -> void override
  {
    audio_events::StartGameplayTheme();
    m_levelContainer->SetPlayerActive(true);
    SetCameraZoom(GetPlayCameraZoom());
  }

  auto End() -> void override
  {
    m_levelContainer->SetPlayerActive(false);
    audio_events::StopPlayerThruster();
    audio_events::StopGameplayTheme();
  }

  auto Pause() -> void override
  {
    audio_events::StopPlayerThruster();
    audio_events::StopGameplayTheme();
    audio_events::StartMainMenuTheme();
  }

  auto Resume() -> void override
  {
    audio_events::StopMainMenuTheme();
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
    renderer::render(m_levelContainer->GameScore());
  #ifdef RENDER_RADAR
    renderer::render(level_radar { m_levelContainer->PlayerPosition() }, m_levelContainer->Targets());
  #endif
  }

private:

  const health_status& m_playerShields;

};
