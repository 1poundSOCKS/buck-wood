#pragma once

#include "framework.h"

class object_outline_data
{
public:

  using points_type = std::vector<game_point>;
  using point_iterator = points_type::const_iterator;
  using point_inserter = std::back_insert_iterator<points_type>;

  object_outline_data()
  {
  }

  auto GetPointInserter() -> point_inserter
  {
    return std::back_inserter(m_points);
  }

  auto Begin() const -> point_iterator
  {
    return m_points.cbegin();
  }

  auto End() const -> point_iterator
  {
    return m_points.cend();
  }

  auto Clear() -> void
  {
    m_points.clear();
  }

private:

  points_type m_points;
};

class collision_data
{
public:

  using point_iterator = object_outline_data::point_iterator;
  using point_inserter = object_outline_data::point_inserter;

  collision_data() : m_data(std::make_shared<object_outline_data>())
  {
  }

  collision_data(const game_closed_object& closedObject) : m_closedObject(closedObject)
  {
  }

  auto GetPointInserter() -> point_inserter
  {
    return m_data->GetPointInserter();
  }

  auto Begin() const -> point_iterator
  {
    return m_data->Begin();
  }

  auto End() const -> point_iterator
  {
    return m_data->End();
  }

  auto Clear() -> void
  {
    m_data->Clear();
  }

  [[nodiscard]] auto PointInside(float x, float y) const -> bool
  {
    return ::PointInside(x, y, m_closedObject);
  }

private:

  game_closed_object m_closedObject;
  std::shared_ptr<object_outline_data> m_data;
};
