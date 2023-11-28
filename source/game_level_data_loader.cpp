#include "pch.h"
#include "game_level_data_loader.h"
#include "level_grid_cell_type.h"

game_level_data_loader::game_level_data_loader()
{
}

auto game_level_data_loader::LoadLevel() -> std::unique_ptr<level_container>
{
  std::unique_ptr<level_container> levelContainer = std::make_unique<level_container>();

  constexpr int gridSize = 800;

  auto levelGrid = level_grid { gridSize, -4, 4, gridSize, -4, 4 };
  
  auto targetView = levelGrid | std::ranges::views::filter([](const auto& cell)
  {
    auto cellType = level_grid_cell_type { cell };
    return cellType.IsTarget();
  });

  levelContainer->AddTargets(targetView);

  auto leftInnerBorder = static_cast<float>(levelGrid.LeftBorder());
  auto topInnerBorder = static_cast<float>(levelGrid.TopBorder());
  auto rightInnerBorder = static_cast<float>(levelGrid.RightBorder());
  auto bottomInnerBorder = static_cast<float>(levelGrid.BottomBorder());

  auto blankObjectPoints = std::array
  {
    game_point { leftInnerBorder, topInnerBorder },
    game_point { rightInnerBorder, topInnerBorder },
    game_point { rightInnerBorder, bottomInnerBorder },
    game_point { leftInnerBorder, bottomInnerBorder }
  };

  auto blankObjects = std::array
  {
    blank_object { blankObjectPoints }
  };

  levelContainer->AddBlankObjects(blankObjects);

  auto solidObjectView = levelGrid |
  std::ranges::views::filter([](const auto& cell)
  {
    auto cellType = level_grid_cell_type { cell };
    return cellType.IsAsteroid();
  }) |
  std::ranges::views::transform([](const auto& cell)
  {
    auto horizontalBorder = cell.Width() / 8;
    auto verticalBorder = cell.Height() / 8;
    return solid_object { cell.Left() + horizontalBorder, cell.Top() + verticalBorder, cell.Right() - horizontalBorder, cell.Bottom() - verticalBorder };
  });

  levelContainer->AddSolidObjects(solidObjectView);

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
