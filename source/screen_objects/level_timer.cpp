#include "pch.h"
#include "level_timer.h"
#include "render.h"
#include "render_brush_defs.h"
#include "render_text_format_def.h"
#include "dwrite_factory.h"
#include "perf_data.h"

inline auto render_text_format_level_timer = render_text_format_def(L"Franklin Gothic", DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 100);

level_timer::level_timer(int64_t value)
{
  m_value = value;
}

auto level_timer::SetTimerUpdate(timer_update timerUpdate) -> void
{
  m_timerUpdate = timerUpdate;
}

auto level_timer::Initialize(ID2D1RenderTarget* renderTarget) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();
  m_brush = screen_render_brush_yellow.CreateBrush(renderTarget);
  auto dwriteFactory = dwrite_factory::get().get();
  m_textFormat = render_text_format_level_timer.CreateTextFormat(dwriteFactory);
}

auto level_timer::Update(const object_input_data& inputData, int64_t clockCount) -> void
{
  auto newValue = m_value - clockCount;
  m_value = max(0, newValue);
  m_timerUpdate(m_value);
}

auto level_timer::Render(D2D1_RECT_F viewRect) const -> void
{
  auto value = static_cast<float>(m_value) / static_cast<float>(performance_counter::QueryFrequency());
  std::wstring timerText = std::format(L"{:.1f}", value);
  RenderText(m_renderTarget.get(), m_brush.get(), m_textFormat.get(), timerText, DWRITE_PARAGRAPH_ALIGNMENT_NEAR, DWRITE_TEXT_ALIGNMENT_TRAILING);
}
