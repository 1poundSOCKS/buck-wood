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

struct render_point
{
  render_point(float x, float y, float size);
  
  D2D1_RECT_F rect;
};

struct render_line
{
  render_line(const D2D1_POINT_2F& start, const D2D1_POINT_2F& end);
  
  D2D1_POINT_2F start, end;
};

void RenderDiagnostics(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const diagnostics_data& diagnosticsData, const dwrite_text_formats& textFormats, const d2d_brushes& brushes);
void RenderTimer(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, float seconds, const dwrite_text_formats& textFormats, const d2d_brushes& brushes);
void RenderMainScreenPrompt(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<IDWriteTextFormat>& textFormat, const winrt::com_ptr<ID2D1SolidColorBrush>& brush, const std::wstring& text);
void RenderMouseCursor(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const mouse_cursor& mouseCursor, float x, float y, const d2d_brushes& brushes);

void RenderPlayer(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform, const player_ship& player, const d2d_brushes& brushes);
void RenderPlayer(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform, const player_ship& player, const winrt::com_ptr<ID2D1SolidColorBrush>& brush, const winrt::com_ptr<ID2D1SolidColorBrush>& brushThrusters);
void RenderBullet(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform, const bullet& bullet, const d2d_brushes& brushes);
void RenderLevel(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform, const game_level& level, const d2d_brushes& brushes);
void RenderLevel(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform, const game_level_edit& level, const d2d_brushes& brushes);

void RenderTarget(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform, const target_edit& target, const winrt::com_ptr<ID2D1SolidColorBrush>& brush);
void RenderShape(const game_shape& shape, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush);
void RenderShape(game_shape_edit& shape, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush);
void RenderLines(const std::list<game_line>& lines, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush);
void RenderLines(const std::list<game_line_edit>& lines, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush);
void RenderPoint(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const render_point& point, const winrt::com_ptr<ID2D1SolidColorBrush>& brush);

void RenderLine(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const render_line& line, float renderWidth, const winrt::com_ptr<ID2D1SolidColorBrush>& brush);

D2D1::Matrix3x2F CreateGameLevelTransform(float centerPosX, float centerPosY, float scale, float renderTargetWidth, float renderTargetHeight);

#endif
