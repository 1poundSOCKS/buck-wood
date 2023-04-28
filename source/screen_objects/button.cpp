#include "pch.h"
#include "button.h"
#include "render_brush_defs.h"
#include "dwrite_factory.h"

button::button(D2D1_RECT_F rect, LPCWSTR text, std::function<void()> eventClicked) : 
  m_buttonHeight { rect.bottom - rect.top },
  m_textFormatDef { L"System Bold", DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, m_buttonHeight * 0.8f },
  m_hoverTextFormatDef { L"System Bold", DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, m_buttonHeight * 0.8f },
  m_text { text }, m_eventClicked { eventClicked }
{
  m_rect = rect;
}

auto button::SetCallbackForHiddenFlag(callback_for_hidden_flag callbackForHiddenFlag) -> void
{
  m_callbackForHiddenFlag = callbackForHiddenFlag;
}

auto button::GetHoverState() const -> bool
{
  return m_hover;  
}

auto button::Initialize(ID2D1RenderTarget* renderTarget) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();
  m_textBrush = screen_render_brush_white.CreateBrush(renderTarget);
  m_buttonBrush = screen_render_brush_grey.CreateBrush(renderTarget);
  m_buttonBorderBrush = screen_render_brush_white.CreateBrush(renderTarget);
  m_buttonHoverBrush = screen_render_brush_green.CreateBrush(renderTarget);
  auto dwriteFactory = dwrite_factory::get().get();
  m_textFormat = m_textFormatDef.CreateTextFormat(dwriteFactory);
  m_hoverTextFormat = m_hoverTextFormatDef.CreateTextFormat(dwriteFactory);
}

auto button::Update(const object_input_data& inputData, int64_t clockCount) -> void
{
  m_hover = IsInsideRect(inputData.GetMouseData().x, inputData.GetMouseData().y, m_rect);

  m_hidden = m_callbackForHiddenFlag();

  if( !m_hidden && m_hover && inputData.LeftMouseButtonClicked() )
  {
    m_eventClicked();
  }
}

auto button::Render(D2D1_RECT_F viewRect) const -> void
{
  if( !m_hidden )
  {
    m_renderTarget->FillRectangle(m_rect, m_buttonBrush.get());
    
    if( m_hover )
    {
      m_renderTarget->DrawRectangle(m_rect, m_buttonHoverBrush.get(), 10.0f);
      RenderText(m_renderTarget.get(), m_buttonHoverBrush.get(), m_hoverTextFormat.get(), m_text, m_rect, 
        DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
    }
    else
    {
      RenderText(m_renderTarget.get(), m_buttonBorderBrush.get(), m_textFormat.get(), m_text, m_rect, 
        DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
    }
  }
}

[[nodiscard]] auto button::IsInsideRect(float x, float y, D2D1_RECT_F rect) -> bool
{
  return ( x < rect.left || x > rect.right || y < rect.top || y > rect.bottom ) ? false : true;
}
