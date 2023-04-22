#include "pch.h"
#include "level_timer.h"
#include "render.h"
#include "render_brush_defs.h"
#include "render_text_format_def.h"

inline auto render_text_format_level_timer = render_text_format_def(L"Franklin Gothic", DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 100);

auto level_timer::control::SetValue(float value) -> void
{
  m_value = value;
}

level_timer::level_timer(control_data controlData)
: m_controlData(controlData)
{
}

auto level_timer::Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();
  m_brush = screen_render_brush_yellow.CreateBrush(renderTarget);
  m_textFormat = render_text_format_level_timer.CreateTextFormat(dwriteFactory);
}

auto level_timer::Update(const object_input_data& inputData, int64_t clockCount) -> void
{
}

auto level_timer::Render(D2D1_RECT_F viewRect) const -> void
{
  std::wstring timerText = std::format(L"{:.1f}", m_controlData->m_value);
  RenderText(m_renderTarget.get(), m_brush.get(), m_textFormat.get(), timerText, DWRITE_PARAGRAPH_ALIGNMENT_NEAR, DWRITE_TEXT_ALIGNMENT_TRAILING);
}
