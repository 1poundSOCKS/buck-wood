#ifndef _render_
#define _render_

#define UNICODE

#include <memory>
#include <tchar.h>

#include "d2d_frame.h"
#include "game_state.h"
#include "perf_data.h"

void RenderFrame(const d2d_frame& frame, const game_state& gameState, const D2D1::Matrix3x2F& transform);
void RenderPlayScreen(const d2d_frame& frame, const play_state& gameState, const D2D1::Matrix3x2F& transform);
void RenderTitleScreen(const d2d_frame& frame);
void RenderLevelEditor(const d2d_frame& frame, const game_state& gameState, const D2D1::Matrix3x2F& transform);
void RenderDiagnostics(const d2d_frame& frame, const std::list<std::wstring>& diagnostics);
void RenderTimer(const d2d_frame& frame, float seconds);
void RenderMouseCursor(const d2d_frame& frame, const mouse_cursor& mouseCursor);
void RenderPlayer(const player_ship& player, const d2d_frame& frame, const D2D1::Matrix3x2F& scaleTransform);
void RenderBullet(const bullet& bullet, const d2d_frame& frame, const D2D1::Matrix3x2F& scaleTransform);
void RenderLevel(const game_level& level, const d2d_frame& frame, const D2D1::Matrix3x2F& scaleTransform);
void RenderShape(const game_shape& shape, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush);
void RenderLines(const std::list<game_line>& lines, const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const winrt::com_ptr<ID2D1SolidColorBrush>& brush);
D2D1::Matrix3x2F CreateViewTransform(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const game_state& gameState);

#endif
