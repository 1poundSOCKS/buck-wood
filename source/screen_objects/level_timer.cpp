#include "pch.h"
#include "level_timer.h"
#include "render.h"
#include "render_defs.h"
#include "clock_frequency.h"

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
  m_textFormat = render_text_format_play_screen_timer.CreateTextFormat(dwriteFactory);
}

auto level_timer::Update(int64_t clockCount) -> void
{
}

auto level_timer::Render(D2D1_RECT_F viewRect) const -> void
{
  std::wstring timerText = std::format(L"{:.2f}", m_controlData->m_value);
  RenderText(m_renderTarget.get(), m_brush.get(), m_textFormat.get(), timerText, DWRITE_PARAGRAPH_ALIGNMENT_NEAR, DWRITE_TEXT_ALIGNMENT_TRAILING);
}
