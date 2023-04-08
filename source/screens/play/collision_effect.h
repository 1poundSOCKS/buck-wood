#pragma once

class collision_effect
{
public:

  enum property { stops_bullets, kills_player };

  auto SetProperty(property propertyName, bool propertyValue) -> void
  {
    switch( propertyName )
    {
      case stops_bullets:
        m_stopsBullets = propertyValue;
        break;
      case kills_player:
        m_killsPlayer = propertyValue;
        break;
      default:
        throw std::exception();
    }
  }

  [[nodiscard]] auto GetProperty(property propertyName) const -> bool
  {
    switch( propertyName )
    {
      case stops_bullets:
        return m_stopsBullets;
      default:
        throw std::exception();
    }
  }

private:

  bool m_stopsBullets = false;
  bool m_killsPlayer = false;
};
