#include "pch.h"
#include "player_ship_renderer.h"

struct render_line
{
  render_line(const D2D1_POINT_2F& start, const D2D1_POINT_2F& end, ID2D1SolidColorBrush* brush, float width = 1);
  
  D2D1_POINT_2F start, end;
  ID2D1SolidColorBrush* brush = nullptr;
  float width = 1;
};

constexpr auto GetPlayerThrusterGeometryData()
{
  constexpr float width { 5 };
  constexpr float yOffset { 16 };
  
  return std::array {
    game_point { width, yOffset },
    game_point { -width, yOffset }
  };
}

template <typename input_iterator_type, typename insert_iterator_type>
void CreateDisconnectedRenderLines(
  typename input_iterator_type begin, 
  typename input_iterator_type end, 
  insert_iterator_type insertIterator, 
  ID2D1SolidColorBrush* brush, 
  float width, 
  float x=0, 
  float y=0)
{
  for( auto i = begin; i != end; std::advance(i, 2) )
  {
    auto next = std::next(i);
    if( next != end ) insertIterator = render_line({i->x + x, i->y + y}, {next->x + x, next->y + y}, brush, width);
  }
};

template <typename input_iterator_type>
void RenderLines(
  ID2D1RenderTarget* renderTarget, 
  input_iterator_type begin, 
  input_iterator_type end)
{
  for( auto line = begin; line != end; ++line )
  {
    renderTarget->DrawLine(line->start, line->end, line->brush, line->width);
  }
}

auto player_ship_renderer::Write(const player_ship& playerShip) const -> void
{
  m_renderer.Write(playerShip.Geometry());

  if( playerShip.ThrusterOn() )
  {
    const auto& position = playerShip.Position();
    auto angle = playerShip.Angle();

    std::vector<game_point> thrusterPoints;
    GetTransformedThrusterGeometry(position.x, position.y, angle, std::back_inserter(thrusterPoints));

    std::vector<render_line> renderLines;
    auto renderLinesInserter = std::back_inserter(renderLines);
    CreateDisconnectedRenderLines(thrusterPoints.cbegin(), thrusterPoints.cend(), renderLinesInserter, m_thrusterBrush.get(), 5);
    RenderLines(render_target::renderTarget().get(), renderLines.cbegin(), renderLines.cend());    
  }
}

auto player_ship_renderer::GetTransformedThrusterGeometry(float x, float y, float angle, std::back_insert_iterator<points_collection> pointsInserter) const -> void
{
  const auto& thrusterGeometryData = GetPlayerThrusterGeometryData();

  TransformPoints(thrusterGeometryData.cbegin(), thrusterGeometryData.cend(), pointsInserter, 
    D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(0,0)) * D2D1::Matrix3x2F::Translation(x, y));
}
