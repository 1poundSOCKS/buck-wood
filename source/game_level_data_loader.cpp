#include "pch.h"
#include "game_level_data_loader.h"
#include "global_state.h"
#include "asteroid_generator.h"

game_level_data_loader::game_level_data_loader() : 
  m_currentLevelDataIterator(global_state::firstLevelData())
{
}

auto game_level_data_loader::LoadLevel(ID2D1RenderTarget* renderTarget) -> std::unique_ptr<level_container>
{
  std::unique_ptr<level_container> levelContainer = std::make_unique<level_container>();
  levelContainer->GetObjectContainer().Initialize(renderTarget);
  levelContainer->AddPlayer(player_ship { 0, 0 });

  level_grid_cell_generator gridCellGenerator;
  const auto cellSize = 300;

  for( int targetIndex = 0; targetIndex < 5; ++targetIndex )
  {
    auto gridCell = gridCellGenerator.Create();
    auto levelTarget = level_target { static_cast<float>(gridCell.x * cellSize), static_cast<float>(gridCell.y * cellSize) };
    levelContainer->AddTarget(levelTarget);
  }

  asteroid_generator asteroidGenerator(cellSize);

  for( int asteroidIndex = 0; asteroidIndex < 20; ++asteroidIndex )
  {
    auto gridCell = gridCellGenerator.Create();
    auto asteroid = asteroidGenerator.Create(gridCell.x, gridCell.y);
    levelContainer->GetObjectContainer().AppendActiveObject(level_island { asteroid });
  }

  levelContainer->SetTimeout(GetTimeLimit());
  levelContainer->GetObjectContainer().Initialize(renderTarget);

  return levelContainer;
}

[[nodiscard]] auto game_level_data_loader::GetTimeLimit() const -> int
{
  return (*m_currentLevelDataIterator)->timeLimitInSeconds;
}

auto game_level_data_loader::NextLevel() -> void
{
}

[[nodiscard]] auto game_level_data_loader::EndOfLevels() const -> bool
{
  return m_currentLevelDataIterator == global_state::endLevelData();
}
