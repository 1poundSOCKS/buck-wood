#ifndef _render_
#define _render_

#define UNICODE

#include <memory>
#include <tchar.h>

#include "framework/framework.h"
#include "d2d_frame.h"
#include "game_objects.h"
#include "diagnostics.h"

void RenderDiagnostics(const d2d_frame& frame, const diagnostics_data& diagnosticsData);
void RenderTimer(const d2d_frame& frame, float seconds);
void RenderMouseCursor(const d2d_frame& frame, const mouse_cursor& mouseCursor);

void RenderPlayer(const d2d_frame& frame, const player_ship& player);
void RenderBullet(const d2d_frame& frame, const bullet& bullet);
void RenderLevel(const d2d_frame& frame, const game_level& level);

void RenderShape(const game_shape& shape, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush);
void RenderLines(const std::list<game_line>& lines, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush);
D2D1::Matrix3x2F CreateGameLevelTransform(float centerPosX, float centerPosY, float scale, float renderTargetWidth, float renderTargetHeight);

#endif
