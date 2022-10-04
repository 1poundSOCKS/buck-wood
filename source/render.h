#ifndef _render_
#define _render_

#define UNICODE

#include <memory>
#include <tchar.h>

#include "d2d_frame.h"
#include "game_state.h"
#include "perf_data.h"

void DoRender(const winrt::com_ptr<ID2D1HwndRenderTarget>& renderTarget, const game_state& gameState, const perf_data& pd, float mouseX, float mouseY);
void RenderMainScreen(const d2d_frame& frame, const game_state& gameState, float mouseX, float mouseY);
void RenderTitleScreen(const d2d_frame& frame);
void RenderDiagnostics(const d2d_frame& frame, const game_state& gameState, const perf_data& pd);
void DrawGameObject(const game_object& gameObject, const d2d_frame& frame, const D2D1::Matrix3x2F& scaleTransform);
void DrawGameObject(const game_object& gameObject, const d2d_frame& frame);
void DrawLevel(const game_level& level, const d2d_frame& frame);
void DrawShape(const shape& shape, const d2d_frame& frame);
std::unique_ptr<D2D1::Matrix3x2F> CreateScaleTransform(const d2d_frame& frame, float screenWidth, float screenHeight);

#endif
