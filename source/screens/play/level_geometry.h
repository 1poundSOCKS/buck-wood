#ifndef _level_geometry_
#define _level_geometry_

#include "game_level_data.h"

struct level_ground_geometry
{
  game_rect boundary;
  game_point groundStart;
  game_point groundEnd;
  std::vector<game_line> lines;
};

struct level_targets_geometry
{
  std::vector<game_point> points;
  std::vector<game_line> lines;
};

[[nodiscard]] auto CreateLevelGroundGeometry(const game_level_data& levelData) -> level_ground_geometry [[nothrow]];
[[nodiscard]] auto CreateLevelTargetsGeometry(const game_level_data& levelData) -> level_targets_geometry [[nothrow]];
[[nodiscard]] auto IsUnderground(float x, float y, const level_ground_geometry& levelGroundGeometry) -> bool [[nothrow]];

namespace level_grid
{
  struct area_state
  {
    enum state_type { not_underground, part_underground, all_underground };
    game_rect rect;
    state_type state;
  };

  [[nodiscard]] auto CreateMatrix(const level_ground_geometry& groundGeometry) -> std::vector<area_state> [[nothrow]];
  
  [[nodiscard]] inline auto SplitRect(
    game_rect rect) -> std::array<game_rect, 4> [[nothrow]]
  {
    auto areaWidth = ( rect.bottomRight.x - rect.topLeft.x ) / 2;
    auto areaHeight = ( rect.bottomRight.y - rect.topLeft.y ) / 2;
    auto areaCentre = game_point { rect.topLeft.x + areaWidth, rect.topLeft.y + areaHeight };

    return std::array {
      game_rect { rect.topLeft.x, rect.topLeft.y, areaCentre.x, areaCentre.y },
      game_rect { areaCentre.x, rect.topLeft.y, rect.bottomRight.x, areaCentre.y },
      game_rect { rect.topLeft.x, areaCentre.y, areaCentre.x, rect.bottomRight.y },
      game_rect { areaCentre.x, areaCentre.y, rect.bottomRight.x, rect.bottomRight.y }
    };
  }

  auto SplitArea(
    const level_grid::area_state& areaState, auto areaInserter, 
    auto GetUndergroundState) -> void
  {
    auto subRects = SplitRect(areaState.rect);

    auto ConvertRectToAreaState = [GetUndergroundState](auto rect) -> level_grid::area_state
    {
      return { rect, GetUndergroundState(rect) };
    };

    std::transform(subRects.cbegin(), subRects.cend(), areaInserter, ConvertRectToAreaState);
  }

  [[nodiscard]] auto SplitMatrixPartials(
    area_state area, 
    auto undergroundAreaInserter, int recursionLevel, auto GetUndergroundState) -> void [[nothrow]]
  {
    auto IsAllUnderground = [](auto area) -> bool
    {
      return area.state == area_state::all_underground;
    };

    auto IsNotAllUnderground = [](auto area) -> bool
    {
      return area.state == area_state::part_underground/* || area.state == area_state::not_underground*/;
    };

    std::vector<level_grid::area_state> areaStates;
    SplitArea(area, std::back_inserter(areaStates), GetUndergroundState);
    std::copy_if(areaStates.cbegin(), areaStates.cend(), undergroundAreaInserter, IsAllUnderground);

    if( --recursionLevel > 0 )
    {
      std::vector<area_state> partUnderground;
      std::copy_if(areaStates.cbegin(), areaStates.cend(), std::back_inserter(partUnderground), IsNotAllUnderground);

      for( auto area : partUnderground )
      {
        SplitMatrixPartials(area, undergroundAreaInserter, recursionLevel, GetUndergroundState);
      }
    }
  }
};

#endif
