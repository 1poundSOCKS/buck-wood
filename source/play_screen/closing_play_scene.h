#pragma once

#include "play_scene.h"
#include "stopwatch.h"
#include "end_of_level_menu_controller.h"
#include "player_state.h"

class closing_play_scene : public play_scene
{

public:

  closing_play_scene(auto&&...args) : play_scene(std::forward<decltype(args)>(args)...)
  {
  }

  auto Begin() -> void override
  {
    m_stopwatch.Reset(m_sceneSeconds);
    SetCameraZoom(GetPlayCameraZoom());
    m_renderTransform = RenderTransform();
  }

  auto Update(int64_t ticks) -> bool override
  {
    play_scene::Update(ticks);

    return m_stopwatch.Update(ticks);
  }

  auto Render() const -> void override
  {
    play_scene::Render();
    
    render_target::get()->SetTransform(D2D1::Matrix3x2F::Identity());
    renderer::render(m_playState->Score());
    game_score powerUps(game_score::value_type::power_ups);
    powerUps.Add(player_state::missile_count());
    renderer::render(powerUps);
  }

private:

  constexpr static float m_sceneSeconds { 3.0f };
  stopwatch m_stopwatch { m_sceneSeconds };

};
