#pragma once

#include "level_title.h"
#include "framework.h"

class level_title_renderer
{

public:

  auto Write(const level_title& levelTitle) const -> void
  {
    text_renderer textRenderer { m_brush, m_defaultText };
    textRenderer.Write(m_rect, DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER, levelTitle.Value().c_str());
  }

private:

  D2D1_SIZE_F m_renderTargetSize { render_target::get()->GetSize() };
  D2D1_RECT_F m_rect { 0, 0, m_renderTargetSize.width - 1, m_renderTargetSize.height - 1 };
  render_brush m_brush { screen_render_brush_green.CreateBrush() };
  render_text_format m_defaultText { render_text_format_def { L"Courier New", DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 100 }.CreateTextFormat() };

};
