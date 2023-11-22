#pragma once

#include "diagnostics.h"
#include "render_types.h"
#include "screen_render_brush_defs.h"
#include "render_text_format_def.h"

class diagnostics_renderer
{

public:

  auto Write() -> void;

private:

  render_brush m_brush { screen_render_brush_white.CreateBrush() };
  render_text_format m_textFormat { render_text_format_def { L"System Bold", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20 }.CreateTextFormat() };

};

inline auto diagnostics_renderer::Write() -> void
{
  const auto& text = diagnostics::text();
  text_renderer textRenderer { m_brush, m_textFormat };
  textRenderer.Write(DWRITE_PARAGRAPH_ALIGNMENT_NEAR, DWRITE_TEXT_ALIGNMENT_LEADING, text.c_str());
}
