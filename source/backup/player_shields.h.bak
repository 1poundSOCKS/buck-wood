#pragma once

#include "objects/player_ship.h"

class player_shields
{

public:

  player_shields() = default;

  auto Attach(const player_ship::shield_status& shieldStatus) -> void;

  [[nodiscard]] auto GetDamagePercentage() const -> float;

private:

  player_ship::shield_status m_shieldStatus;

};

inline auto player_shields::Attach(const player_ship::shield_status& shieldStatus) -> void
{
  m_shieldStatus = shieldStatus;
}

[[nodiscard]] inline auto player_shields::GetDamagePercentage() const -> float
{
  return m_shieldStatus ? m_shieldStatus->GetDamagePercentage() : 0;
}
