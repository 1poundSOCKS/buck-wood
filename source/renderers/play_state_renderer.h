#pragma once

#include "framework.h"
#include "render_types.h"
#include "render_text_format_def.h"
#include "screen_render_brush_defs.h"
#include "play_state.h"

class play_state_renderer
{

public:

  auto Write(const play_state& playState) const noexcept -> void;

private:

  SIZE_F m_renderTargetSize { render_target::get()->GetSize() };
  RECT_F m_rect { 0, 0, m_renderTargetSize.width / 2 - 100, 50 };
  render_text_format m_defaultText { render_text_format_def { L"Courier New", DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 50 }.CreateTextFormat() };
  render_brush m_brushTotal { screen_render_brush_blue.CreateBrush() };
  text_renderer m_textRenderer { m_brushTotal, m_defaultText };

};
