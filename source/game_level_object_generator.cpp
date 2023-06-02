#include "pch.h"
#include "game_level_object_generator.h"
#include "framework.h"
#include "level_asteroid.h"

game_level_object_generator::game_level_object_generator(int minColumn, int maxColumn, int columnWidth, int minRow, int maxRow, int rowHeight, float noiseLower, float noiseUpper, float noiseDial) :
  m_minColumn(minColumn), m_maxColumn(maxColumn), m_columnWidth(columnWidth), m_minRow(minRow), m_maxRow(maxRow), m_rowHeight(rowHeight), m_noiseLower(noiseLower), m_noiseUpper(noiseUpper), m_noiseDial(noiseDial)
{
}

auto game_level_object_generator::InsertInto(std::back_insert_iterator<star_collection> starInserter) const -> void
{
  for( auto row = m_minRow; row <= m_maxRow; ++row )
  {
    for( auto column = m_minColumn; column <= m_maxColumn; ++ column )
    {
      auto x = static_cast<float>(column * m_columnWidth);
      auto y = static_cast<float>(row * m_rowHeight);

      auto noiseValue = psn::GetNoise(x / m_noiseDial, y / m_noiseDial);

      auto rowRadius = m_rowHeight / 2;
      auto columnRadius = m_columnWidth / 2;

      auto rect = game_rect { { x - columnRadius, y - rowRadius }, { x + columnRadius, y + rowRadius } };
      
      if( noiseValue >= m_noiseLower && noiseValue < m_noiseUpper )
      {
        starInserter = game_point { x, y };
      }
    }
  }
}

auto game_level_object_generator::InsertInto(std::back_insert_iterator<target_collection> targetInserter) const -> void
{
  for( auto row = m_minRow; row <= m_maxRow; ++row )
  {
    for( auto column = m_minColumn; column <= m_maxColumn; ++ column )
    {
      auto x = static_cast<float>(column * m_columnWidth);
      auto y = static_cast<float>(row * m_rowHeight);

      auto noiseValue = psn::GetNoise(x / m_noiseDial, y / m_noiseDial);

      auto rowRadius = m_rowHeight / 2;
      auto columnRadius = m_columnWidth / 2;

      auto rect = game_rect { { x - columnRadius, y - rowRadius }, { x + columnRadius, y + rowRadius } };
      
      if( noiseValue >= m_noiseLower && noiseValue < m_noiseUpper )
      {
        targetInserter = level_target { x, y };
      }
    }
  }
}

[[nodiscard]] auto game_level_object_generator::CreateTarget(game_rect rect) const -> game_closed_object
{
  auto width = ( rect.bottomRight.x - rect.topLeft.x ) / 8.0f;
  auto height = ( rect.bottomRight.y - rect.topLeft.y ) / 8.0f;

  auto centrePoint = rect.CentrePoint();
  auto x = centrePoint.x;
  auto y = centrePoint.y;

  std::vector<game_point> points;

  for( int angle = 0; angle < 360; angle += 60 )
  {
    auto angleInRadians = DEGTORAD(angle);

    auto cx = width * sin(angleInRadians);
    auto cy = height * cos(angleInRadians);

    points.emplace_back( game_point {x + cx, y + cy} );
  }

  game_closed_object object;
  object.Load(points.cbegin(), points.cend());

  return object;
}
