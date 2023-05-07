#include "pch.h"
#include "game_level_data_loader.h"
#include "global_state.h"
#include "asteroid_generator.h"

game_level_data_loader::game_level_data_loader()
{
}

auto game_level_data_loader::LoadLevel(ID2D1RenderTarget* renderTarget) -> std::unique_ptr<level_container>
{
  std::unique_ptr<level_container> levelContainer = std::make_unique<level_container>(renderTarget);
  // levelContainer->GetObjectContainer().Initialize(renderTarget);
  levelContainer->AddPlayer(player_ship { 0, 0 });

  level_grid_cell_generator gridCellGenerator;

  asteroid_generator asteroidGenerator(50, 40, 40);
  
  asteroid_generator::asteroid_collection asteroids;
  asteroidGenerator.InsertInto(std::back_inserter(asteroids));

  for( const auto& asteroid : asteroids )
  {
    levelContainer->AddAsteroid(asteroid);
  }

  asteroid_generator::target_collection targets;
  asteroidGenerator.InsertInto(std::back_inserter(targets));

  for( const auto& target : targets )
  {
    levelContainer->AddTarget(target);
  }

  levelContainer->SetTimeout(GetTimeLimit());
  // levelContainer->GetObjectContainer().Initialize(renderTarget);

  return levelContainer;
}

[[nodiscard]] auto game_level_data_loader::GetTimeLimit() const -> int
{
  return 60;
}

auto game_level_data_loader::NextLevel() -> void
{
  ++m_levelIndex;
}

[[nodiscard]] auto game_level_data_loader::EndOfLevels() const -> bool
{
  return m_levelIndex > 0;
}
