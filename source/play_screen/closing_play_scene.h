#pragma once

#include "play_scene.h"
#include "stopwatch.h"
#include "end_of_level_menu_controller.h"
#include "player_state.h"

class closing_play_scene : public play_scene
{

public:

  closing_play_scene(auto&&...args);

  auto Begin(const level_container& levelContainer) -> void override;
  auto Update(const level_container& levelContainer, int64_t ticks) -> void override;
  auto Render(const level_container& levelContainer, const play_state& playState) const -> void override;
  auto Complete(const level_container& levelContainer, const play_state& playState) const -> bool override;

private:

  constexpr static float m_sceneSeconds { 3.0f };
  stopwatch m_stopwatch { m_sceneSeconds };
  bool m_complete { false };

};

closing_play_scene::closing_play_scene(auto&&...args) : play_scene(std::forward<decltype(args)>(args)...)
{
}

inline auto closing_play_scene::Begin(const level_container& levelContainer) -> void
{
  m_stopwatch.Reset(m_sceneSeconds);
  SetCameraZoom(GetPlayCameraZoom());
  m_renderTransform = RenderTransform(levelContainer);
}

inline auto closing_play_scene::Update(const level_container& levelContainer, int64_t ticks) -> void
{
  play_scene::Update(levelContainer, ticks);
  m_complete = !m_stopwatch.Update(ticks);
}

inline auto closing_play_scene::Render(const level_container& levelContainer, const play_state& playState) const -> void
{
  play_scene::Render(levelContainer, playState);
  
  render_target::get()->SetTransform(D2D1::Matrix3x2F::Identity());
  renderer::render(playState.Score());
  game_score powerUps(game_score::value_type::power_ups);
  powerUps.Add(player_state::missile_count());
  renderer::render(powerUps);
}

inline auto closing_play_scene::Complete(const level_container& levelContainer, const play_state& playState) const -> bool
{
  return m_complete;
}
