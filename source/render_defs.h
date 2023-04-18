#pragma once

#include "render_brush_def.h"
#include "render_text_format_def.h"

inline auto screen_render_brush_white = render_brush_def(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f));
inline auto screen_render_brush_grey = render_brush_def(D2D1::ColorF(0.4f, 0.4f, 0.4f, 1.0f));
inline auto screen_render_brush_dark_grey = render_brush_def(D2D1::ColorF(0.1f, 0.1f, 0.1f, 1.0f));
inline auto screen_render_brush_green = render_brush_def(D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f));
inline auto screen_render_brush_red = render_brush_def(D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f));
inline auto screen_render_brush_yellow = render_brush_def(D2D1::ColorF(1.0f, 1.0f, 0.0f, 1.0f));
inline auto screen_render_brush_cyan = render_brush_def(D2D1::ColorF(0.0f, 1.0f, 1.0f, 1.0f));
inline auto screen_render_brush_brown = render_brush_def(D2D1::ColorF(0.6f, 0.3f, 0.0f, 1.0f));

inline auto render_text_format_prompt = render_text_format_def(L"Verdana", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20);
inline auto render_text_format_main_menu = render_text_format_def(L"Verdana", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20);
inline auto render_text_format_play_screen_state = render_text_format_def(L"Franklin Gothic", DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 80);
inline auto render_text_format_play_screen_timer = render_text_format_def(L"Franklin Gothic", DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 100);
