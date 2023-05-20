#include "pch.h"
#include "text_box.h"
#include "render.h"
#include "render_brush_defs.h"
#include "render_text_format_def.h"
#include "dwrite_factory.h"
#include "framework.h"

inline auto render_text_format_text_box = render_text_format_def(L"Franklin Gothic", DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 100);

text_box::text_box(render_target_area renderTargetArea) : m_renderTargetArea(renderTargetArea)
{
  Initialize(framework::renderTarget().get());
}

auto text_box::SetTextGetter(text_getter textGetter) -> void
{
  m_textGetter = textGetter;
}

auto text_box::SetCallbackForHiddenFlag(callback_for_flag callbackForHiddenFlag) -> void
{
  m_callbackForHiddenFlag = callbackForHiddenFlag;
}

auto text_box::Initialize(ID2D1RenderTarget* renderTarget) -> void
{
  m_brush = screen_render_brush_yellow.CreateBrush(renderTarget);
  const auto& dwriteFactory = dwrite_factory::get();
  m_textFormat = render_text_format_text_box.CreateTextFormat(dwriteFactory.get());
}

auto text_box::Update(const object_input_data& inputData, int64_t clockCount) -> void
{
  m_textValue = m_textGetter();
  m_hidden = m_callbackForHiddenFlag();
}

auto text_box::Render(D2D1_RECT_F viewRect) const -> void
{
  if( !m_hidden && m_textValue.length() > 0 )
  {
    RenderText(framework::renderTarget().get(), m_brush.get(), m_textFormat.get(), m_textValue.c_str(), m_renderTargetArea.GetRect(), DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
  }
}
