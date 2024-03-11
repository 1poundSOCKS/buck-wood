#pragma once

class reload_timer
{

public:

  reload_timer(float reloadTime);
  auto Update(float interval) -> bool;
  auto Reset() -> void;

  [[nodiscard]] auto Interval() -> float { return m_reloadTime; }

private:

  float m_reloadTime;
  float m_currentTime;

};
