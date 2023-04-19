#include "pch.h"
#include "level_state.h"
#include "render.h"
#include "render_defs.h"

level_state::level_state()
: m_sharedData(std::make_shared<shared_data_type>())
{
}

auto level_state::Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void
{
  m_brush = screen_render_brush_cyan.CreateBrush(renderTarget);
  m_textFormat = render_text_format_play_screen_state.CreateTextFormat(dwriteFactory);
}

auto level_state::Update(int64_t clockCount) -> void
{
}

auto level_state::Render(D2D1_RECT_F viewRect) const -> void
{
  switch( m_sharedData->state )
  {
    case paused:
      RenderText(m_renderTarget.get(), m_brush.get(), m_textFormat.get(), L"PAUSED", DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
      break;
    case dead:
      RenderText(m_renderTarget.get(), m_brush.get(), m_textFormat.get(), L"DEAD", DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
      break;
    case complete:
      RenderText(m_renderTarget.get(), m_brush.get(), m_textFormat.get(), L"LEVEL COMPLETE", DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
      break;
  }
}

auto level_state::SetState(state_type state) -> void
{
  m_sharedData->state = state;  
}

auto level_state::GetState() const -> state_type
{
  return m_sharedData->state;
}

std::wstring GetGameCompleteMsg(const std::vector<float>& levelTimes)
{
  std::wstring msg;
  for( auto levelTime: levelTimes ) { msg += std::format(L"{:.2f}", levelTime); }
  return msg;
}
