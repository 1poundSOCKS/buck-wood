#pragma once

#include "framework.h"

using points_type = std::vector<game_point>;

class object_outline_data
{
public:

  object_outline_data()
  {
  }

  auto GetPointInserter() -> std::back_insert_iterator<points_type>
  {
    return std::back_inserter(m_points);
  }

private:

  points_type m_points;
};

class object_outline
{
public:

  object_outline()
  {
  }

private:

  std::shared_ptr<object_outline_data> m_data;
};
