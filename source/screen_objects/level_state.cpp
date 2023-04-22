#include "pch.h"
#include "level_state.h"
#include "render.h"
#include "render_brush_defs.h"
#include "render_text_format_def.h"

inline auto render_text_format_level_state = render_text_format_def(L"Franklin Gothic", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 80);

auto level_state::control::SetState(state_type stateValue) -> void
{
  m_state = stateValue;
}

auto level_state::control::GetText() const -> LPCWSTR
{
  switch( m_state )
  {
    case paused:
      return L"PAUSED";
    case dead:
      return L"DEAD";
    case complete:
      return L"COMPLETE";
    default:
      return nullptr;
  }
}

level_state::level_state() : m_controlData(std::make_shared<control>())
{
}

auto level_state::GetControlData() const -> control_data
{
  return m_controlData;
}

auto level_state::Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();
  m_brush = screen_render_brush_cyan.CreateBrush(renderTarget);
  m_textFormat = render_text_format_level_state.CreateTextFormat(dwriteFactory);
}

auto level_state::Update(const object_input_data& inputData, int64_t clockCount) -> void
{
}

auto level_state::Render(D2D1_RECT_F viewRect) const -> void
{
  LPCWSTR renderText = m_controlData->GetText();

  if( renderText )
  {
    RenderText(m_renderTarget.get(), m_brush.get(), m_textFormat.get(), renderText, DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
  }
}

std::wstring GetGameCompleteMsg(const std::vector<float>& levelTimes)
{
  std::wstring msg;
  for( auto levelTime: levelTimes ) { msg += std::format(L"{:.2f}", levelTime); }
  return msg;
}
