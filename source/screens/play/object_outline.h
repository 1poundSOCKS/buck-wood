#pragma once

#include "framework.h"

class object_outline_data
{
public:

  using points_type = std::vector<game_point>;
  using point_iterator = points_type::iterator;
  using point_inserter = std::back_insert_iterator<points_type>;

  object_outline_data()
  {
  }

  auto GetPointInserter() -> point_inserter
  {
    return std::back_inserter(m_points);
  }

  auto Begin() -> point_iterator
  {
    return m_points.begin();
  }

  auto End() -> point_iterator
  {
    return m_points.end();
  }

  auto Clear() -> void
  {
    m_points.clear();
  }

private:

  points_type m_points;
};

class object_outline
{
public:

  using point_iterator = object_outline_data::point_iterator;
  using point_inserter = object_outline_data::point_inserter;

  object_outline() : m_data(std::make_shared<object_outline_data>())
  {
  }

  auto GetPointInserter() -> point_inserter
  {
    return m_data->GetPointInserter();
  }

  auto Begin() -> point_iterator
  {
    return m_data->Begin();
  }

  auto End() -> point_iterator
  {
    return m_data->End();
  }

  auto Clear() -> void
  {
    m_data->Clear();
  }

private:

  std::shared_ptr<object_outline_data> m_data;
};
