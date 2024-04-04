#pragma once

#include "play_scene.h"
#include "renderers.h"
#include "audio_events.h"

class main_play_scene : public play_scene
{

public:

  main_play_scene(auto&&...args);
  ~main_play_scene();

  auto Begin() -> void override;
  auto End() -> void override;
  auto Pause() -> void override;
  auto Resume() -> void override;
  auto Update(int64_t ticks) -> bool override;
  auto Render() const -> void override;

};

main_play_scene::main_play_scene(auto&&...args) : play_scene(std::forward<decltype(args)>(args)...)
{
}
