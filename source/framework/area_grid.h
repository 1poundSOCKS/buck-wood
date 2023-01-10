#ifndef _area_state_
#define _area_state_

#include "geometry.h"

struct area_state
{
  enum state_type { omit, split, keep };
  state_type state;
  game_rect rect;
};

[[nodiscard]] inline auto QuarterRect(game_rect rect) -> std::array<game_rect, 4>
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

void QuarterArea(const area_state& areaState, auto areaInserter, auto GetAreaState)
{
  auto subRects = QuarterRect(areaState.rect);

  auto ConvertRectToAreaState = [GetAreaState](auto rect) -> area_state
  {
    return { GetAreaState(rect), rect };
  };

  std::transform(subRects.cbegin(), subRects.cend(), areaInserter, ConvertRectToAreaState);
}

void SplitArea(area_state area, int recursionCount, auto areaStateInserter, auto GetAreaState)
{
  auto Keep = [](auto area) -> bool
  { return area.state == area_state::keep; };
  
  auto Split = [](auto area) -> bool
  { return area.state == area_state::split; };

  std::vector<area_state> areaStates;
  QuarterArea(area, std::back_inserter(areaStates), GetAreaState);
  std::copy_if(areaStates.cbegin(), areaStates.cend(), areaStateInserter, Keep);

  if( --recursionCount > 0 )
  {
    std::vector<area_state> areasToSplit;
    std::copy_if(areaStates.cbegin(), areaStates.cend(), std::back_inserter(areasToSplit), Split);

    for( auto areaToSplit : areasToSplit )
    {
      SplitArea(areaToSplit, recursionCount, areaStateInserter, GetAreaState);
    }
  }
  else
  {
    std::copy_if(areaStates.cbegin(), areaStates.cend(), areaStateInserter, Split);
  }
}

void SplitArea(game_rect area, int recursionCount, auto areaStateInserter, auto GetAreaState)
{
  auto areaState = area_state { area_state::split, area };
  SplitArea(areaState, recursionCount, areaStateInserter, GetAreaState);
}

#endif
