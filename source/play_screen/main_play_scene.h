#pragma once

#include "play_scene.h"
#include "renderers.h"
#include "audio_events.h"

class main_play_scene : public play_scene
{

public:

  main_play_scene(auto&&...args) : play_scene(std::forward<decltype(args)>(args)...)
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
    SetCameraZoom(GetPlayCameraZoom());
    m_playState->LevelContainer().SetPlayerActive(true);
    m_playState->LevelContainer().SetPlayerInvulnerable(false);
  }

  auto End() -> void override
  {
    m_playState->LevelContainer().SetPlayerActive(false);
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
    renderer::render(m_playState->Score());
    renderer::render(m_playState->PowerUps());
  #ifdef RENDER_RADAR
    renderer::render(level_radar { m_playState->LevelContainer().PlayerPosition() }, m_playState->LevelContainer().Targets());
  #endif
  }

};
