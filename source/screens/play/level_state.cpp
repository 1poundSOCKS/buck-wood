#include "pch.h"
#include "level_state.h"
#include "render.h"

level_state::level_state(screen_render_brush_selector brushes, screen_render_text_format_selector textFormats)
: m_sharedData(std::make_shared<shared_data_type>())
{
  m_brush.attach(brushes[cyan]);
  m_brush->AddRef();
  m_textFormat.attach(textFormats[srtf_play_screen_state]);
  m_textFormat->AddRef();
}

auto level_state::Update(int64_t clockFrequency, int64_t clockCount) -> void
{

}

auto level_state::RenderTo(ID2D1RenderTarget* renderTarget) const -> void
{
  switch( m_sharedData->state )
  {
    case paused:
      RenderText(
        renderTarget, 
        m_brush.get(), 
        m_textFormat.get(), 
        L"PAUSED", 
        DWRITE_PARAGRAPH_ALIGNMENT_CENTER, 
        DWRITE_TEXT_ALIGNMENT_CENTER);
      break;
    case dead:
      RenderText(
        renderTarget, 
        m_brush.get(), 
        m_textFormat.get(), 
        L"DEAD", 
        DWRITE_PARAGRAPH_ALIGNMENT_CENTER, 
        DWRITE_TEXT_ALIGNMENT_CENTER);
      break;
    case complete:
      RenderText(
        renderTarget, 
        m_brush.get(), 
        m_textFormat.get(), 
        L"LEVEL COMPLETE", 
        DWRITE_PARAGRAPH_ALIGNMENT_CENTER, 
        DWRITE_TEXT_ALIGNMENT_CENTER);
      break;
    // case complete:
    //   RenderText(
    //     renderTarget, 
    //     renderBrushSelector[cyan], 
    //     textFormatSelector[srtf_play_screen_state], 
    //     GetGameCompleteMsg(screenState.levelTimes), 
    //     DWRITE_PARAGRAPH_ALIGNMENT_CENTER, 
    //     DWRITE_TEXT_ALIGNMENT_CENTER);
    //   break;
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
