#pragma once

class reload_timer
{

public:

  reload_timer(float reloadTime);
  auto Update(float interval) -> bool;

  [[nodiscard]] auto Interval() -> float { return m_reloadTime; }

private:

  float m_reloadTime;
  float m_currentTime;

};
