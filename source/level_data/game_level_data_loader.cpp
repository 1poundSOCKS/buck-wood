#include "pch.h"
#include "game_level_data_loader.h"
#include "demo_level.h"

game_level_data_loader::game_level_data_loader()
{
}

auto game_level_data_loader::LoadLevel() -> std::unique_ptr<level_container>
{
  demo_level demoLevel;

  std::unique_ptr<level_container> levelContainer = std::make_unique<level_container>(demoLevel.BoundaryPoints());
  levelContainer->AddTargets(demoLevel.TargetPositions());
  levelContainer->AddAsteroids(demoLevel.AsteroidPositions());
  levelContainer->AddDuctFans(demoLevel.DuctFanPositions());

  return levelContainer;
}

[[nodiscard]] auto game_level_data_loader::NextLevel() -> bool
{
  return ++m_levelIndex < m_levelCount;
}
