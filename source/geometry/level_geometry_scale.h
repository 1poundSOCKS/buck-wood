#pragma once

#include "level_geometries.h"

class level_geometry_scale
{

public:

  static auto create() -> void
  {
    destroy();
    m_instance = new level_geometry_scale();
  }

  static auto destroy() -> void
  {
    if( m_instance )
    {
      delete m_instance;
      m_instance = nullptr;
    }
  }

  static [[nodiscard]] auto player() -> SCALE_2F
  {
    return m_instance->m_playerScale;
  }

private:

  level_geometry_scale();

  static [[nodiscard]] auto Scale(ID2D1Geometry* geometry, SIZE_F objectSize) -> SCALE_2F;

private:

  inline static level_geometry_scale* m_instance { nullptr };

  SCALE_2F m_playerScale;

};
