#include "pch.h"
#include "game_level_data_loader.h"
#include "perlin_simplex_noise.h"

game_level_data_loader::game_level_data_loader()
{
}

auto game_level_data_loader::LoadLevel() -> std::unique_ptr<level_container>
{
  std::unique_ptr<level_container> levelContainer = std::make_unique<level_container>();
  
  level_grid targetGrid { level_container::CellWidth(), level_container::CellHeight(), -4000, -4000, 4000, 4000 };
  
  auto view = targetGrid | std::ranges::views::filter([](const auto& cell)
  {
    auto noise = psn::GetNoise(static_cast<float>(cell.x), static_cast<float>(cell.y));
    return noise < -0.9;
  });

  levelContainer->AddTargets(view);

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
