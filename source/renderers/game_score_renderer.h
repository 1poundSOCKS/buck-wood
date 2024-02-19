#pragma once

#include "game_score.h"

class game_score_renderer
{

public:

  auto Write(const game_score& gameScore) const -> void
  {
    std::wstring score = std::format(L"{}", gameScore.Value());
    text_renderer textRenderer { m_brush, m_defaultText };
    textRenderer.Write(m_rect, DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER, score.c_str());
  }

private:

  render_brush m_brush { screen_render_brush_green.CreateBrush() };
  render_text_format m_defaultText { render_text_format_def { L"Courier New", DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 100 }.CreateTextFormat() };
  D2D1_RECT_F m_rect { 0, 0, 200, 200 };

};
