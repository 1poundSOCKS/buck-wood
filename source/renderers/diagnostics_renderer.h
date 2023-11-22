#pragma once

#include "diagnostics.h"
#include "render_types.h"
#include "screen_render_brush_defs.h"
#include "render_text_format_def.h"

class diagnostics_text_renderer : public text_renderer
{

public:

  diagnostics_text_renderer() : 
    text_renderer { render_brush { screen_render_brush_white.CreateBrush() },
    render_text_format { render_text_format_def { L"System Bold", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 18 }.CreateTextFormat() },
    DWRITE_PARAGRAPH_ALIGNMENT_NEAR, DWRITE_TEXT_ALIGNMENT_LEADING }
  {
  }

};

class diagnostics_renderer
{

public:

  auto Write() -> void;

private:

  diagnostics_text_renderer m_textRenderer;

};

inline auto diagnostics_renderer::Write() -> void
{
  const auto& text = diagnostics::text();
  m_textRenderer.Write(text.c_str());
}
