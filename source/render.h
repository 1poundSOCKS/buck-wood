#ifndef _render_
#define _render_

#define UNICODE

#include <memory>
#include <tchar.h>

#include "framework/framework.h"
#include "game_objects.h"
#include "diagnostics.h"
#include "dwrite_text_formats.h"
#include "d2d_brushes.h"

void RenderDiagnostics(const d2d_frame& frame, const diagnostics_data& diagnosticsData, const dwrite_text_formats& textFormats, const d2d_brushes& brushes);
void RenderTimer(const d2d_frame& frame, float seconds, const dwrite_text_formats& textFormats, const d2d_brushes& brushes);
void RenderMouseCursor(const d2d_frame& frame, const mouse_cursor& mouseCursor, const d2d_brushes& brushes);

void RenderPlayer(const d2d_frame& frame, const player_ship& player, const d2d_brushes& brushes);
void RenderBullet(const d2d_frame& frame, const bullet& bullet, const d2d_brushes& brushes);
void RenderLevel(const d2d_frame& frame, const game_level& level, const d2d_brushes& brushes);
void RenderLevel(const d2d_frame& frame, const game_level_edit& level, const d2d_brushes& brushes);
void RenderHighlightedPoint(const d2d_frame& frame, const game_point& point, const d2d_brushes& brushes);

void RenderShape(const game_shape& shape, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush);
void RenderShape(const game_shape_edit& shape, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush);
void RenderLines(const std::list<game_line>& lines, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush);
void RenderLines(const std::list<game_line_edit>& lines, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush);

D2D1::Matrix3x2F CreateGameLevelTransform(float centerPosX, float centerPosY, float scale, float renderTargetWidth, float renderTargetHeight);

#endif
