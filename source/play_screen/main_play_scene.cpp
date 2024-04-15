#include "pch.h"
#include "main_play_scene.h"
#include "player_state.h"

main_play_scene::~main_play_scene()
{
  audio_events::StopPlayerThruster();
  audio_events::StopGameplayTheme();
}

auto main_play_scene::Begin() -> void
{
  audio_events::StartGameplayTheme();
  SetCameraZoom(GetPlayCameraZoom());
  m_renderTransform = RenderTransform();
}

auto main_play_scene::End() -> void
{
  audio_events::StopPlayerThruster();
  audio_events::StopGameplayTheme();
}

auto main_play_scene::Pause() -> void
{
  audio_events::StopPlayerThruster();
  audio_events::StopGameplayTheme();
  audio_events::StartMainMenuTheme();
}

auto main_play_scene::Resume() -> void
{
  audio_events::StopMainMenuTheme();
  audio_events::StartGameplayTheme();
}

auto main_play_scene::Update(int64_t ticks) -> bool
{
  return play_scene::Update(ticks);
}

auto main_play_scene::Render() const -> void
{
  play_scene::Render();

  render_target::get()->SetTransform(D2D1::Matrix3x2F::Identity());
  
  renderer::render(m_playState->Score());

  game_score powerUps(game_score::value_type::power_ups);
  powerUps.Add(player_state::missile_count());
  renderer::render(powerUps);

#ifdef RENDER_RADAR
  renderer::render(level_radar { m_playState->LevelContainer().PlayerPosition() }, m_playState->LevelContainer().Targets());
#endif
}
