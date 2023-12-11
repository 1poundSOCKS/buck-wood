#include "pch.h"
#include "game_level_data_loader.h"
#include "demo_level.h"

game_level_data_loader::game_level_data_loader()
{
}

auto game_level_data_loader::LoadLevel() -> std::unique_ptr<level_container>
{
  std::unique_ptr<level_container> levelContainer = std::make_unique<level_container>();

  demo_level demoLevel;

  levelContainer->AddBlankObjects(std::array { blank_object { demoLevel.Boundary() } } );
  levelContainer->AddTargets(demoLevel.Targets());
  levelContainer->AddAsteroids(demoLevel.Asteroids());
  levelContainer->AddDuctFans(demoLevel.DuctFans());

  return levelContainer;
}

[[nodiscard]] auto game_level_data_loader::NextLevel() -> bool
{
  return ++m_levelIndex < m_levelCount;
}
