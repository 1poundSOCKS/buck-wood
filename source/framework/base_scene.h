#pragma once

#include "level_container.h"
#include "play_state.h"

class base_scene
{

public:

  virtual auto Begin(const level_container& levelContainer) -> void
  {
  }

  virtual auto End(const level_container& levelContainer) -> void
  {
  }

  virtual auto Pause(const level_container& levelContainer) -> void
  {
  }

  virtual auto Resume(const level_container& levelContainer) -> void
  {
  }

  virtual auto Update(const level_container& levelContainer, int64_t ticks) -> void
  {
  }

  virtual auto Render(const level_container& levelContainer, const play_state& playState) const -> void
  {
  }

  virtual auto GetRenderTargetView(const level_container& levelContainer) const -> D2D1_RECT_F
  {
    return { 0.0f, 0.0f, 0.0f, 0.0f };
  }

  virtual auto Complete(const level_container& levelContainer, const play_state& playState) const -> bool
  {
    return true;
  }

};
