#pragma once

#include "render_brush_def.h"

inline auto screen_render_brush_white = render_brush_def(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f));
inline auto screen_render_brush_grey = render_brush_def(D2D1::ColorF(0.4f, 0.4f, 0.4f, 1.0f));
inline auto screen_render_brush_dark_grey = render_brush_def(D2D1::ColorF(0.1f, 0.1f, 0.1f, 1.0f));
inline auto screen_render_brush_green = render_brush_def(D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f));
inline auto screen_render_brush_red = render_brush_def(D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f));
inline auto screen_render_brush_yellow = render_brush_def(D2D1::ColorF(1.0f, 1.0f, 0.0f, 1.0f));
inline auto screen_render_brush_cyan = render_brush_def(D2D1::ColorF(0.0f, 1.0f, 1.0f, 1.0f));
inline auto screen_render_brush_brown = render_brush_def(D2D1::ColorF(0.6f, 0.3f, 0.0f, 1.0f));
