#include "pch.h"
#include "game_level_data_loader.h"
#include "level_grid_cell_type.h"
#include "demo_level.h"

game_level_data_loader::game_level_data_loader()
{
}

auto game_level_data_loader::LoadLevel() -> std::unique_ptr<level_container>
{
  std::unique_ptr<level_container> levelContainer = std::make_unique<level_container>();

  constexpr int gridSize = 800;
  auto levelGrid = level_grid { gridSize, -4, 4, gridSize, -4, 4 };
  
  demo_level demoLevel { gridSize, gridSize };

  levelContainer->AddBlankObjects(std::array { blank_object { demoLevel.Boundary() } } );
  levelContainer->AddTargets(demoLevel.Targets());
  levelContainer->SetCentre(levelGrid.Centre());
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
