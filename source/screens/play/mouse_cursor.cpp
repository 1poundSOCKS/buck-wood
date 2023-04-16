#include "pch.h"
#include "mouse_cursor.h"
#include "screen_render.h"

mouse_cursor::mouse_cursor(screen_render_brush_selector brushes)
: m_sharedData(std::make_shared<shared_data_type>())
{
  m_brush.attach(brushes[white]);
  m_brush->AddRef();
}

auto mouse_cursor::Update(int64_t clockFrequency, int64_t clockCount) -> void
{

}

auto mouse_cursor::RenderTo(ID2D1RenderTarget* renderTarget) const -> void
{
  RenderMouseCursor(renderTarget, m_brush.get(), m_sharedData->x, m_sharedData->y);
}

auto mouse_cursor::SetPosition(float x, float y) -> void
{
  m_sharedData->x = x;
  m_sharedData->y = y;
}
