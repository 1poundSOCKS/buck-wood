#include "pch.h"
#include "d2d_factory.h"
#include "screen_render.h"

[[nodiscard]] auto d2d_factory::createPathGeometry() -> winrt::com_ptr<ID2D1PathGeometry>
{
  return CreatePathGeometry(m_instance->m_factory.get());
}
