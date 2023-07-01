#pragma once

#include "player_ship.h"

class player_shields
{

public:

  player_shields() = default;

  auto Attach(const player_ship::shield_status& shieldStatus) -> void;

private:

  player_ship::shield_status m_shieldStatus;

};

inline auto player_shields::Attach(const player_ship::shield_status& shieldStatus) -> void
{
  m_shieldStatus = shieldStatus;
}
