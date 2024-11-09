#pragma once

#include "play_scene.h"
#include "stopwatch.h"
#include "end_of_level_menu_controller.h"

class final_play_scene : public play_scene
{

public:

  final_play_scene(auto&&...args);

  auto Begin(const level_container& levelContainer) -> void override;
  auto Update(const level_container& levelContainer, int64_t ticks) -> void override;
  auto Render(const level_container& levelContainer, const play_state& playState) const -> void override;
  auto Complete(const level_container& levelContainer, const play_state& playState) const -> bool override;

private:

  constexpr static float m_sceneSeconds { 3.0f };
  stopwatch m_stopwatch { m_sceneSeconds };
  end_of_level_menu_controller m_menuController;
  bool m_complete { false };

};

final_play_scene::final_play_scene(auto&&...args) : play_scene(std::forward<decltype(args)>(args)...)
{
  m_menuController.OpenRoot();
}

inline auto final_play_scene::Begin(const level_container& levelContainer) -> void
{
  m_stopwatch.Reset(m_sceneSeconds);
  SetCameraZoom(GetPlayCameraZoom());
  m_renderTransform = RenderTransform(levelContainer);
}

inline auto final_play_scene::Update(const level_container& levelContainer, int64_t ticks) -> void
{
  play_scene::Update(levelContainer, ticks);

  if( !m_paused && m_stopwatch.Expired() )
  {
    m_menuController.Update();
  }

  m_complete = m_stopwatch.Update(ticks) || m_menuController.Selection() == end_of_level_menu_controller::selection::NONE ? false : true;
}

auto final_play_scene::Render(const level_container& levelContainer, const play_state& playState) const -> void
{
  play_scene::Render(levelContainer, playState);
  
  render_target::get()->SetTransform(D2D1::Matrix3x2F::Identity());
  renderer::render(playState.Score());
  game_score powerUps(game_score::value_type::power_ups);
  powerUps.Add(player_state::missile_count());
  renderer::render(powerUps);

  if( !m_paused && m_stopwatch.Expired() )
  {
    D2D1_SIZE_F renderTargetSize = render_target::get()->GetSize();
    m_menuController.Render(D2D1_RECT_F { 0, 0, renderTargetSize.width - 1, renderTargetSize.height - 1});
  }
}

inline auto final_play_scene::Complete(const level_container& levelContainer, const play_state& playState) const -> bool
{
  return m_complete;
}
