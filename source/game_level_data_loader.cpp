#include "pch.h"
#include "game_level_data_loader.h"
#include "level_grid_cell_type.h"

game_level_data_loader::game_level_data_loader()
{
}

auto game_level_data_loader::LoadLevel() -> std::unique_ptr<level_container>
{
  std::unique_ptr<level_container> levelContainer = std::make_unique<level_container>();

  auto levelGrid = levelContainer->GetGrid(-6000, -4000, 6000, 4000);
  
  auto targetView = levelGrid | std::ranges::views::filter([](const auto& cell)
  {
    auto cellType = level_grid_cell_type { cell };
    return cellType.IsTarget();
  });

  levelContainer->AddTargets(targetView);

  auto solidObjectView = levelGrid |
  std::ranges::views::filter([](const auto& cell)
  {
    auto cellType = level_grid_cell_type { cell };
    return cellType.IsAsteroid();
  }) |
  std::ranges::views::transform([](const auto& cell)
  {
    return solid_object { cell.Left(), cell.Top(), cell.Right(), cell.Bottom() };
  });

  levelContainer->AddSolidObjects(solidObjectView);

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
