#include "pch.h"
#include "button.h"
#include "render_brush_defs.h"

button::button(D2D1_RECT_F rect, LPCWSTR text, std::function<void()> eventClicked) : 
  m_textFormatDef { L"Franklin Gothic", DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, rect.bottom - rect.top },
  m_text { text },
  m_eventClicked(eventClicked)
{
  m_rect = rect;
}

auto button::Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();
  m_textBrush = screen_render_brush_white.CreateBrush(renderTarget);
  m_buttonBrush = screen_render_brush_grey.CreateBrush(renderTarget);
  m_buttonBorderBrush = screen_render_brush_white.CreateBrush(renderTarget);
  m_buttonHoverBrush = screen_render_brush_yellow.CreateBrush(renderTarget);
  m_textFormat = m_textFormatDef.CreateTextFormat(dwriteFactory);
}

auto button::Update(const object_input_data& inputData, int64_t clockCount) -> void
{
  m_hover = IsInsideRect(inputData.GetMouseData().x, inputData.GetMouseData().y, m_rect);

  if( m_hover && inputData.LeftMouseButtonClicked() )
  {
    m_eventClicked();
  }
}

auto button::Render(D2D1_RECT_F viewRect) const -> void
{
  m_renderTarget->FillRectangle(m_rect, m_buttonBrush.get());
  
  if( m_hover )
  {
    m_renderTarget->DrawRectangle(m_rect, m_buttonHoverBrush.get(), 5.0f);
    RenderText(m_renderTarget.get(), m_buttonHoverBrush.get(), m_textFormat.get(), m_text, m_rect, DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
  }
  else
  {
    m_renderTarget->DrawRectangle(m_rect, m_buttonBorderBrush.get(), 5.0f);
    RenderText(m_renderTarget.get(), m_buttonBorderBrush.get(), m_textFormat.get(), m_text, m_rect, DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
  }
}

[[nodiscard]] auto button::IsInsideRect(float x, float y, D2D1_RECT_F rect) -> bool
{
  return ( x < rect.left || x > rect.right || y < rect.top || y > rect.bottom ) ? false : true;
}
