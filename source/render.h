#ifndef _render_
#define _render_

#define UNICODE

#include <memory>
#include <tchar.h>

#include "d2d_frame.h"
#include "game_state.h"
#include "perf_data.h"

void RenderFrame(const d2d_frame& frame, const game_state& gameState, const D2D1::Matrix3x2F& transform);
void RenderMainScreen(const d2d_frame& frame, const game_state& gameState, const D2D1::Matrix3x2F& transform);
void RenderTitleScreen(const d2d_frame& frame);
void RenderDiagnostics(const d2d_frame& frame, const game_state& gameState, const perf_data& pd);
void DrawPlayer(const player_ship& player, const d2d_frame& frame, const D2D1::Matrix3x2F& scaleTransform);
void DrawBullet(const bullet& bullet, const d2d_frame& frame, const D2D1::Matrix3x2F& scaleTransform);
void DrawLevel(const game_level& level, const d2d_frame& frame, const D2D1::Matrix3x2F& scaleTransform);
void DrawShape(const game_shape& shape, const d2d_frame& frame);
D2D1::Matrix3x2F CreateViewTransform(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, float widthToScale, float heightToTranslate);

#endif
