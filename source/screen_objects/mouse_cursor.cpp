#include "pch.h"
#include "mouse_cursor.h"
#include "screen_render.h"
#include "render_defs.h"

mouse_cursor::mouse_cursor() : m_sharedData(std::make_shared<shared_data_type>())
{
}

auto mouse_cursor::Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();
  m_brush = screen_render_brush_white.CreateBrush(renderTarget);
}

auto mouse_cursor::Update(int64_t clockCount) -> void
{
}

auto mouse_cursor::Render(D2D1_RECT_F viewRect) const -> void
{
  RenderMouseCursor(m_renderTarget.get(), m_brush.get(), m_sharedData->x, m_sharedData->y);
}

auto mouse_cursor::SetPosition(float x, float y) -> void
{
  m_sharedData->x = x;
  m_sharedData->y = y;
}
