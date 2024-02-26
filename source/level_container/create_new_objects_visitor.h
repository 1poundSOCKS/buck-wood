#pragma once

#include "level_container.h"

struct create_new_objects_visitor
{
  level_container& m_levelContainer;

  auto operator()(auto& object)
  {
    m_levelContainer.CreateNewObjects(object);
  }
};
