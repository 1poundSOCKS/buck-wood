#include "pch.h"
#include "mouse_cursor.h"
#include "screen_render.h"
#include "render_brush_defs.h"

consteval std::array<D2D1_POINT_2F, 8> GetCursorRenderData()
{
  const float cursorSize = 20.0f;
  const float cursorSizeGap = 10.0f;

  return std::array<D2D1_POINT_2F, 8> {
    D2D1_POINT_2F { 0, -cursorSize },
    D2D1_POINT_2F { 0,-cursorSizeGap },
    D2D1_POINT_2F { 0,cursorSize },
    D2D1_POINT_2F { 0,cursorSizeGap },
    D2D1_POINT_2F { -cursorSize,0 },
    D2D1_POINT_2F { -cursorSizeGap,0 },
    D2D1_POINT_2F { cursorSize,0 },
    D2D1_POINT_2F { cursorSizeGap,0 }
  };
}

mouse_cursor::mouse_cursor()
{
}

auto mouse_cursor::Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();
  m_brush = screen_render_brush_white.CreateBrush(renderTarget);
}

auto mouse_cursor::Update(const object_input_data& inputData, int64_t clockCount) -> void
{
  m_x = inputData.GetMouseData().x;
  m_y = inputData.GetMouseData().y;
}

auto mouse_cursor::Render(D2D1_RECT_F viewRect) const -> void
{
  static const float cursorSize = 20.0f;
  static const float cursorSizeGap = 10.0f;

  auto mouseCursor = GetCursorRenderData();

  std::vector<render_line> renderLines;
  
  CreateDisconnectedRenderLines(
    mouseCursor.cbegin(), 
    mouseCursor.cend(), 
    std::back_inserter(renderLines), 
    m_brush.get(), 5, m_x, m_y);
  
  RenderLines(m_renderTarget.get(), renderLines.cbegin(), renderLines.cend());
}
