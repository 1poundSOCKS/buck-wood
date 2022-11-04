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
void RenderMouseCursor(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const mouse_cursor& mouseCursor, float x, float y, const d2d_brushes& brushes);

void RenderPlayer(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform, const player_ship& player, const d2d_brushes& brushes);
void RenderBullet(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform, const bullet& bullet, const d2d_brushes& brushes);
void RenderLevel(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform, const game_level& level, const d2d_brushes& brushes);
void RenderLevel(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform, const game_level_edit& level, const d2d_brushes& brushes);
void RenderHighlightedPoint(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform, const game_point& point, const d2d_brushes& brushes);

void RenderShape(const game_shape& shape, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush);
void RenderShape(game_shape_edit& shape, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush);
void RenderLines(const std::list<game_line>& lines, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush);
void RenderLines(const std::list<game_line_edit>& lines, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush);

D2D1::Matrix3x2F CreateGameLevelTransform(float centerPosX, float centerPosY, float scale, float renderTargetWidth, float renderTargetHeight);

#endif
