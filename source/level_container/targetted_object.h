#pragma once

#include "level_objects.h"

class targetted_object
{

public:

  targetted_object(mine object) : m_object { object }
  {
  }

  operator mine() const
  {
    return m_object;
  }

  [[nodiscard]] auto Position() const -> game_point
  {
    return m_object.Position();
  }

private:

  mine m_object;

};
