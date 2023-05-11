#include "pch.h"
#include "game_level_data_loader.h"
#include "global_state.h"
#include "game_level_object_generator.h"
#include "level_asteroid_container.h"

game_level_data_loader::game_level_data_loader()
{
}

auto game_level_data_loader::LoadLevel(ID2D1RenderTarget* renderTarget) -> std::unique_ptr<level_container>
{
  std::unique_ptr<level_container> levelContainer = std::make_unique<level_container>(renderTarget);
  
  levelContainer->AddPlayer(player_ship { 0, 0 });

  game_level_object_generator objectGenerator( { { -2000, -2000 } , { 2000, 2000 } } );

  game_level_object_generator::asteroid_collection asteroids;
  game_level_object_generator::target_collection targets;
  
  objectGenerator.InsertInto(std::back_inserter(asteroids), std::back_inserter(targets));

  for( const auto& asteroid : asteroids )
  {
    levelContainer->AddAsteroid(asteroid);
  }
  
  for( const auto& target : targets )
  {
    levelContainer->AddTarget(target);
  }

  levelContainer->SetTimeout(GetTimeLimit());

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
