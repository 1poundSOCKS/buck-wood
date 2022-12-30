#ifndef _geometry_
#define _geometry_

#define PI 3.14159265
#define DEGTORAD(D)((D * PI) / 180.0)
#define RADTODEG(R)((180.0 * R) / PI)

struct game_point
{
  float x = 0;
  float y = 0;
};

struct game_line
{
  game_point start;
  game_point end;
};

struct game_rect
{
  game_point topLeft;
  game_point bottomRight;
};

int CalculateAngle(float x1, float y1, float x2, float y2);

float GetDistanceBetweenPoints(float x1, float y1, float x2, float y2);

[[nodiscard]] auto GetBoundingRect(game_line line) -> game_rect [[nothrow]];
[[nodiscard]] auto DoOverlap(game_rect rect1, game_rect rect2) -> bool [[nothrow]];
[[nodiscard]] auto GetCentrePoint(game_rect rect) -> game_point [[nothrow]];

void TransformPoints(auto begin, auto end, auto transformedPoints, const D2D1::Matrix3x2F& transform)
{
  std::transform(begin, end, transformedPoints, [transform](const auto& point){
    auto outPoint = transform.TransformPoint({point.x, point.y});
    return decltype(transformedPoints)::container_type::value_type ( outPoint.x, outPoint.y );
  });
}

void CreateConnectedLines(auto begin, auto end, auto lines, float x=0, float y=0, bool loop=true)
{
  std::transform(std::next(begin), end, begin, lines, [x, y](const auto& point2, const auto& point1)
  {
    return game_line { point1.x + x, point1.y + y, point2.x + x, point2.y + y };
  });

  if( loop )
  {
    auto last = std::prev(end);
    lines = game_line { last->x + x, last->y + y, begin->x + x, begin->y + y };
  }
};

struct area_state
{
  enum state_type { omit, split, keep };
  state_type state;
  game_rect rect;
};

[[nodiscard]] inline auto QuarterRect(
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

auto QuarterArea(
  const area_state& areaState, auto areaInserter, 
  auto GetAreaState) -> void
{
  auto subRects = QuarterRect(areaState.rect);

  auto ConvertRectToAreaState = [GetAreaState](auto rect) -> area_state
  {
    return { GetAreaState(rect), rect };
  };

  std::transform(subRects.cbegin(), subRects.cend(), areaInserter, ConvertRectToAreaState);
}

[[nodiscard]] auto SplitAreaPartials(
  area_state area, 
  auto undergroundAreaInserter, int recursionCount, auto GetAreaState) -> void [[nothrow]]
{
  auto Keep = [](auto area) -> bool
  { return area.state == area_state::keep; };
  
  auto Split = [](auto area) -> bool
  { return area.state == area_state::split; };

  std::vector<area_state> areaStates;
  QuarterArea(area, std::back_inserter(areaStates), GetAreaState);
  std::copy_if(areaStates.cbegin(), areaStates.cend(), undergroundAreaInserter, Keep);

  if( --recursionCount > 0 )
  {
    std::vector<area_state> areasToSplit;
    std::copy_if(areaStates.cbegin(), areaStates.cend(), std::back_inserter(areasToSplit), Split);

    for( auto area : areasToSplit )
    {
      SplitAreaPartials(area, undergroundAreaInserter, recursionCount, GetAreaState);
    }
  }
  // else
  // {
  //   std::copy_if(areaStates.cbegin(), areaStates.cend(), undergroundAreaInserter, Split);
  // }
}

[[nodiscard]] auto SplitArea(game_rect area, int recursionCount, auto GetAreaState)
-> std::vector<area_state> [[nothrow]]
{
  auto areaState = area_state { area_state::split, area };

  std::vector<area_state> areaStates;
  SplitAreaPartials(areaState, std::back_inserter(areaStates), recursionCount, GetAreaState);
  return areaStates;
}

#endif
