#include "pch.h"
#include "level_timer.h"
#include "render.h"
#include "render_defs.h"
#include "clock_frequency.h"

level_timer::level_timer(control_shared_type controlData)
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
  // if( !m_sharedData->stopped )
  // {
  //   if( m_sharedData->timeRemaining == 0 )
  //   {
  //     m_sharedData->timeRemaining = m_sharedData->levelTime * clock_frequency::get();
  //   }
  //   else
  //   {
  //     m_sharedData->timeRemaining -= clockCount;
  //   }
  // }
}

auto level_timer::Render(D2D1_RECT_F viewRect) const -> void
{
  std::wstring timerText = std::format(L"{:.2f}", m_controlData->value);
  RenderText(m_renderTarget.get(), m_brush.get(), m_textFormat.get(), timerText, DWRITE_PARAGRAPH_ALIGNMENT_NEAR, DWRITE_TEXT_ALIGNMENT_TRAILING);
}

// [[nodiscard]] auto level_timer::HasExpired() const -> bool
// {
//   return m_sharedData->timeRemaining > 0 ? false : true;
// }

// [[nodiscard]] auto level_timer::GetTimeRemainingInSeconds() const -> float
// {
  // auto timeRemaining = static_cast<float>(m_sharedData->timeRemaining) / static_cast<float>(clock_frequency::get());
  // return max(timeRemaining, 0.0f);
// }

// auto level_timer::Stop() -> void
// {
//   m_sharedData->stopped = true;
// }
