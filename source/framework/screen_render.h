#ifndef _screen_render_
#define _screen_render_

using screen_render_brushes = std::vector<winrt::com_ptr<ID2D1SolidColorBrush>>;

enum screen_render_brush_color { white=0, grey, green, red, yellow, cyan };

struct screen_render_brush_selector
{
  screen_render_brushes& brushes;
  ID2D1SolidColorBrush* operator[](screen_render_brush_color brushColor)
  {
    return brushes[brushColor].get();
  }
};

using screen_render_text_formats = std::vector<winrt::com_ptr<IDWriteTextFormat>>;

enum screen_render_text_format { diagnostics=0 };

struct screen_render_text_format_selector
{
  screen_render_text_formats& textFormats;
  IDWriteTextFormat* operator[](screen_render_text_format textFormat)
  {
    return textFormats[textFormat].get();
  }
};

void CreateScreenRenderBrushes(ID2D1RenderTarget* renderTarget, screen_render_brushes& brushes);
void CreateScreenRenderTextFormats(screen_render_text_formats& textFormats);
void RenderMouseCursor(ID2D1RenderTarget* renderTarget, ID2D1SolidColorBrush* brush, float x, float y);
void RenderDiagnostics(ID2D1RenderTarget* renderTarget, screen_render_brush_selector brushSelector, screen_render_text_format_selector textFormatSelector, const std::vector<std::wstring>& diagnosticsData);

#endif
