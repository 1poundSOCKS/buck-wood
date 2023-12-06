#pragma once

class base_scene
{

public:

  virtual auto Begin() -> void
  {
  }

  virtual auto End() -> void
  {
  }

  virtual auto Pause() -> void
  {
  }

  virtual auto Resume() -> void
  {
  }

  virtual auto Update(__int64 ticks) -> bool
  {
    return false;
  }

  virtual auto Render() const -> void
  {
  }

};
