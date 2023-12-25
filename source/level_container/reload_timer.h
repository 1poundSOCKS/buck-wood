#pragma once

class reload_timer
{

public:

  reload_timer(float reloadTime);
  auto Update(float interval) -> bool;

private:

  float m_reloadTime;
  float m_currentTime;

};
