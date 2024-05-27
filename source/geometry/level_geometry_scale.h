#pragma once

#include "level_geometries.h"

class level_geometry_scale
{

public:

  static auto create() -> void;
  static auto destroy() -> void;

  static [[nodiscard]] auto player() -> SCALE_2F;
  static [[nodiscard]] auto playerBullet() -> SCALE_2F;

private:

  level_geometry_scale();

  static [[nodiscard]] auto Scale(ID2D1Geometry* geometry, SIZE_F objectSize) -> SCALE_2F;

private:

  inline static level_geometry_scale* m_instance { nullptr };

  SCALE_2F m_player;
  SCALE_2F m_playerBullet;

};
