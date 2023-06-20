#include "pch.h"
#include "level_radar.h"
#include "screen_render_brush_defs.h"

level_radar::level_radar() : m_brushes { color_scale { D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f), D2D1::ColorF(0.0f, 0.5f, 0.0f, 1.0f), 10 } }
{
}
