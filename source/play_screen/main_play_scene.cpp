#include "pch.h"
#include "main_play_scene.h"
#include "player_state.h"

main_play_scene::~main_play_scene()
{
  audio_events::StopPlayerThruster();
  audio_events::StopGameplayTheme();
}

auto main_play_scene::Begin(const level_container& levelContainer) -> void
{
  audio_events::StartGameplayTheme();
  SetCameraZoom(GetPlayCameraZoom());
  m_renderTransform = RenderTransform(levelContainer);
}

auto main_play_scene::End(const level_container& levelContainer) -> void
{
  audio_events::StopPlayerThruster();
  audio_events::StopGameplayTheme();
}

auto main_play_scene::Pause(const level_container& levelContainer) -> void
{
  audio_events::StopPlayerThruster();
  audio_events::StopGameplayTheme();
  audio_events::StartMainMenuTheme();
}

auto main_play_scene::Resume(const level_container& levelContainer) -> void
{
  audio_events::StopMainMenuTheme();
  audio_events::StartGameplayTheme();
}

auto main_play_scene::Update(const level_container& levelContainer, int64_t ticks) -> void
{
  play_scene::Update(levelContainer, ticks);
}

auto main_play_scene::Render(const level_container& levelContainer, const play_state& playState) const -> void
{
  play_scene::Render(levelContainer, playState);

  render_target::get()->SetTransform(D2D1::Matrix3x2F::Identity());
  
  renderer::render(playState.Score());

  game_score powerUps(game_score::value_type::power_ups);
  powerUps.Add(player_state::missile_count());
  renderer::render(powerUps);

#ifdef RENDER_RADAR
  renderer::render(level_radar { m_playState->LevelContainer().PlayerPosition() }, m_playState->LevelContainer().Targets());
#endif
}

auto main_play_scene::Complete(const level_container& levelContainer, const play_state& playState) const -> bool
{
  return playState.LevelOver(levelContainer);
}
