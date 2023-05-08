#pragma once

#include "geometry.h"

class path_geometry
{

public:

  path_geometry();
  path_geometry(const game_closed_object& object);
  auto Get() const -> ID2D1PathGeometry*;
  auto Load(const game_closed_object& object) -> void;

private:

  winrt::com_ptr<ID2D1PathGeometry> m_geometry;

};
