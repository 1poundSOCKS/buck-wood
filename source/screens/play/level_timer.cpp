#include "pch.h"
#include "level_timer.h"
#include "render.h"

level_timer::level_timer(screen_render_brush_selector brushes, screen_render_text_format_selector textFormats, int timeInSeconds)
: m_sharedData(std::make_shared<shared_data_type>())
{
  m_sharedData->levelTime = timeInSeconds;
  m_brush.attach(brushes[yellow]);
  m_brush->AddRef();
  m_textFormat.attach(textFormats[srtf_play_screen_timer]);
  m_textFormat->AddRef();
}

auto level_timer::Update(int64_t clockFrequency, int64_t clockCount) -> void
{
  if( !m_sharedData->stopped )
  {
    if( m_sharedData->timeRemaining == 0 )
    {
      m_sharedData->timeRemaining = m_sharedData->levelTime * clockFrequency;
      m_sharedData->clockFrequency = clockFrequency;
    }
    else
    {
      m_sharedData->timeRemaining -= clockCount;
    }
  }
}

auto level_timer::RenderTo(ID2D1RenderTarget* renderTarget) const -> void
{
  auto levelTimeRemaining = GetTimeRemainingInSeconds();
  
  std::wstring timerText = std::format(L"{:.2f}", levelTimeRemaining);
  
  RenderText(
    renderTarget, 
    m_brush.get(),
    m_textFormat.get(), 
    timerText, 
    DWRITE_PARAGRAPH_ALIGNMENT_NEAR, 
    DWRITE_TEXT_ALIGNMENT_TRAILING);
}

[[nodiscard]] auto level_timer::HasExpired() const -> bool
{
  return m_sharedData->timeRemaining > 0 ? false : true;
}

[[nodiscard]] auto level_timer::GetTimeRemainingInSeconds() const -> float
{
  auto timeRemaining = static_cast<float>(m_sharedData->timeRemaining) / static_cast<float>(m_sharedData->clockFrequency);
  return max(timeRemaining, 0.0f);
}

auto level_timer::Stop() -> void
{
  m_sharedData->stopped = true;
}
