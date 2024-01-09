#pragma once

#include "geometry_containment.h"
#include "particle_containment.h"

#include "level_objects.h"

class level_containment_checks
{

public:

  using explosion_collection = std::vector<game_point>;
  using impact_collection = std::vector<game_point>;

public:

  geometry_containment<mine> mineContainment { [this](auto& mine)
  {
    m_explosions.emplace_back(mine->PreviousPosition());
    mine->Destroy();
  }};

  geometry_containment<player_ship> shipContainment { [this](auto& ship)
  {
    m_explosions.emplace_back(ship->PreviousPosition());
    ship->ApplyFatalDamage();
  }};

  [[nodiscard]] auto Explosions() const -> const explosion_collection&
  {
    return m_explosions;
  }

  [[nodiscard]] auto Impacts() const -> const impact_collection&
  {
    return m_impacts;
  }

  auto Reset() -> void
  {
    m_explosions.clear();
    m_impacts.clear();
  }

private:

  explosion_collection m_explosions;
  impact_collection m_impacts;

};
