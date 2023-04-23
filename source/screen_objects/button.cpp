#include "pch.h"
#include "button.h"
#include "render_brush_defs.h"

auto button::control::Hide() -> void
{
  m_hidden = true;
}

auto button::control::Unhide() -> void
{
  m_hidden = false;
}

button::button(D2D1_RECT_F rect, LPCWSTR text, std::function<void()> eventClicked, bool hidden) : 
  m_controlData(std::make_shared<control>()),
  m_buttonHeight { rect.bottom - rect.top },
  m_textFormatDef { L"Verdana", DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, m_buttonHeight * 0.8f },
  m_hoverTextFormatDef { L"Verdana", DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, m_buttonHeight * 0.8f },
  m_text { text }, m_eventClicked { eventClicked }
{
  m_rect = rect;
}

auto button::GetControlData() const -> control_data
{
  return m_controlData;
}

auto button::Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();
  m_textBrush = screen_render_brush_white.CreateBrush(renderTarget);
  m_buttonBrush = screen_render_brush_grey.CreateBrush(renderTarget);
  m_buttonBorderBrush = screen_render_brush_white.CreateBrush(renderTarget);
  m_buttonHoverBrush = screen_render_brush_green.CreateBrush(renderTarget);
  m_textFormat = m_textFormatDef.CreateTextFormat(dwriteFactory);
  m_hoverTextFormat = m_hoverTextFormatDef.CreateTextFormat(dwriteFactory);
}

auto button::Update(const object_input_data& inputData, int64_t clockCount) -> void
{
  m_hover = IsInsideRect(inputData.GetMouseData().x, inputData.GetMouseData().y, m_rect);

  if( !m_controlData->m_hidden )
  {
    if( m_hover && inputData.LeftMouseButtonClicked() )
    {
      m_eventClicked();
    }
  }
}

auto button::Render(D2D1_RECT_F viewRect) const -> void
{
  if( !m_controlData->m_hidden )
  {
    // m_renderTarget->FillRectangle(m_rect, m_buttonBrush.get());
    
    if( m_hover )
    {
      m_renderTarget->DrawRectangle(m_rect, m_buttonHoverBrush.get(), 10.0f);
      RenderText(m_renderTarget.get(), m_buttonHoverBrush.get(), m_hoverTextFormat.get(), m_text, m_rect, 
        DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
    }
    else
    {
      m_renderTarget->DrawRectangle(m_rect, m_buttonBorderBrush.get(), 10.0f);
      RenderText(m_renderTarget.get(), m_buttonBorderBrush.get(), m_textFormat.get(), m_text, m_rect, 
        DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
    }
  }
}

[[nodiscard]] auto button::IsInsideRect(float x, float y, D2D1_RECT_F rect) -> bool
{
  return ( x < rect.left || x > rect.right || y < rect.top || y > rect.bottom ) ? false : true;
}
