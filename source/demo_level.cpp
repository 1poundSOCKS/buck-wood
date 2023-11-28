#include "pch.h"
#include "demo_level.h"

demo_level::demo_level(const level_grid& levelGrid)
{
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

  std::ranges::copy(blankObjectPoints, std::back_inserter(m_points));
}

auto demo_level::begin() const -> std::vector<game_point>::const_iterator
{
  return std::begin(m_points);
}

auto demo_level::end() const -> std::vector<game_point>::const_iterator
{
  return std::end(m_points);
}
