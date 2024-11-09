#pragma once

#include "play_scene.h"
#include "renderers.h"
#include "audio_events.h"

class main_play_scene : public play_scene
{

public:

  main_play_scene(auto&&...args);
  ~main_play_scene();

  auto Begin(const level_container& levelContainer) -> void override;
  auto End(const level_container& levelContainer) -> void override;
  auto Pause(const level_container& levelContainer) -> void override;
  auto Resume(const level_container& levelContainer) -> void override;
  auto Update(const level_container& levelContainer, int64_t ticks) -> void override;
  auto Render(const level_container& levelContainer, const play_state& playState) const -> void override;
  auto Complete(const level_container& levelContainer, const play_state& playState) const -> bool override;

};

main_play_scene::main_play_scene(auto&&...args) : play_scene(std::forward<decltype(args)>(args)...)
{
}
