#include "pch.h"
#include "game_level_data_loader.h"
#include "game_level_object_generator.h"

game_level_data_loader::game_level_data_loader()
{
}

auto game_level_data_loader::LoadLevel() -> std::unique_ptr<level_container>
{
  std::unique_ptr<level_container> levelContainer = std::make_unique<level_container>();
  
  game_level_object_generator targetGenerator(-20, 20, 100, -20, 20, 100, -1.0f, -0.97f, 17.0f);
  game_level_object_generator::target_collection targets;
  targetGenerator.InsertInto(std::back_inserter(targets));

  levelContainer->AddTargets(targets);

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
