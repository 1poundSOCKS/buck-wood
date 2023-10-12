#include "pch.h"
#include "game_level_data_loader.h"
#include "level_grid_cell_type.h"

game_level_data_loader::game_level_data_loader()
{
}

auto game_level_data_loader::LoadLevel() -> std::unique_ptr<level_container>
{
  std::unique_ptr<level_container> levelContainer = std::make_unique<level_container>();

  auto levelGrid = level_grid { 800, -4, 4, 800, -4, 4 };
  
  auto targetView = levelGrid | std::ranges::views::filter([](const auto& cell)
  {
    auto cellType = level_grid_cell_type { cell };
    return cellType.IsTarget();
  });

  levelContainer->AddTargets(targetView);

  auto leftBorder = static_cast<float>(levelGrid.LeftBorder());
  auto topBorder = static_cast<float>(levelGrid.TopBorder());
  auto rightBorder = static_cast<float>(levelGrid.RightBorder());
  auto bottomBorder = static_cast<float>(levelGrid.BottomBorder());

  constexpr float borderSize = 1000;

  auto leftBorderObjectPoints = std::array
  {
    game_point { leftBorder - borderSize, topBorder - borderSize},
    game_point { leftBorder, topBorder },
    game_point { leftBorder, bottomBorder },
    game_point { leftBorder - borderSize, bottomBorder + borderSize }
  };

  auto topBorderObjectPoints = std::array
  {
    game_point { leftBorder - borderSize, topBorder - borderSize},
    game_point { rightBorder + borderSize, topBorder - borderSize },
    game_point { rightBorder, topBorder },
    game_point { leftBorder, topBorder }
  };

  auto rightBorderObjectPoints = std::array
  {
    game_point { rightBorder + borderSize, topBorder - borderSize},
    game_point { rightBorder, topBorder },
    game_point { rightBorder, bottomBorder },
    game_point { rightBorder + borderSize, bottomBorder + borderSize }
  };

  auto bottomBorderObjectPoints = std::array
  {
    game_point { leftBorder - borderSize, bottomBorder + borderSize},
    game_point { rightBorder + borderSize, bottomBorder + borderSize },
    game_point { rightBorder, bottomBorder },
    game_point { leftBorder, bottomBorder }
  };

  auto borderObjects = std::array
  {
    solid_object { leftBorderObjectPoints },
    solid_object { topBorderObjectPoints },
    solid_object { rightBorderObjectPoints },
    solid_object { bottomBorderObjectPoints }
  };

  levelContainer->AddSolidObjects(borderObjects);

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
