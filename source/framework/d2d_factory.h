#pragma once

#include "screen_render.h"

class d2d_factory
{

public:

  static auto create() -> void;
  static auto destroy() -> void;

  [[nodiscard]] static auto get() -> const winrt::com_ptr<ID2D1Factory>&;
  [[nodiscard]] static auto get_raw() -> ID2D1Factory*;
  [[nodiscard]] static auto createPathGeometry() -> winrt::com_ptr<ID2D1PathGeometry>;

private:

  d2d_factory();

private:

  inline static d2d_factory* m_instance { nullptr };
  winrt::com_ptr<ID2D1Factory> m_factory;

};

inline auto d2d_factory::create() -> void
{
  destroy();
  m_instance = new d2d_factory();
}

inline auto d2d_factory::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

[[nodiscard]] inline auto d2d_factory::get() -> const winrt::com_ptr<ID2D1Factory>&
{
  return m_instance->m_factory;
}

[[nodiscard]] inline auto d2d_factory::get_raw() -> ID2D1Factory*
{
  return m_instance->m_factory.get();
}

inline d2d_factory::d2d_factory()
{
  m_factory = CreateD2DFactory();
}
