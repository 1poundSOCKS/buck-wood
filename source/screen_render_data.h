#ifndef _screen_render_data_
#define _screen_render_data_

using screen_render_brushes = std::vector<winrt::com_ptr<ID2D1SolidColorBrush>>;

enum screen_render_brush_color { white=0, grey, dark_grey, green, red, yellow, cyan, brown };

struct screen_render_brush_selector
{
  const screen_render_brushes& brushes;
  ID2D1SolidColorBrush* operator[](screen_render_brush_color brushColor) const
  {
    return brushes[brushColor].get();
  }
};

using screen_render_text_formats = std::vector<winrt::com_ptr<IDWriteTextFormat>>;

enum screen_render_text_format { srtf_prompt = 0, srtf_main_menu, srtf_play_screen_state, srtf_play_screen_timer };

struct screen_render_text_format_selector
{
  const screen_render_text_formats& textFormats;
  IDWriteTextFormat* operator[](screen_render_text_format textFormat) const
  {
    return textFormats[textFormat].get();
  }
};

struct screen_render_data
{
  screen_render_brushes renderBrushes;
  screen_render_text_formats textFormats;
};

[[nodiscard]] auto CreateScreenRenderBrushes(ID2D1RenderTarget* renderTarget) -> screen_render_brushes;
[[nodiscard]] auto CreateScreenRenderTextFormats(IDWriteFactory* dwriteFactory) -> screen_render_text_formats;

#endif
