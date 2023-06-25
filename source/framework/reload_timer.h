#pragma once

class reload_timer
{

public:

  reload_timer() = default;
  reload_timer(float reloadTime);
  auto Update(float interval) -> bool;

private:

  float m_reloadTime { 0 };
  float m_currentTime { 0 };
};
