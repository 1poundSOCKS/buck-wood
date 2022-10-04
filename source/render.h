#define UNICODE

#include <memory>
#include <tchar.h>

#include "d2d_frame.h"
#include "game_state.h"
#include "perf_data.h"

void DoRender(const winrt::com_ptr<ID2D1HwndRenderTarget>& renderTarget, const game_state& gameState, const std::unique_ptr<perf_data>& pd, float mouseX, float mouseY);
void RenderMainScreen(const std::unique_ptr<d2d_frame>& frame, const game_state& gameState, float mouseX, float mouseY);
void RenderTitleScreen(const std::unique_ptr<d2d_frame>& frame);
void RenderDiagnostics(const std::unique_ptr<d2d_frame>& frame, const game_state& gameState, const std::unique_ptr<perf_data>& pd);
void DrawGameObject(const game_object& gameObject, const std::unique_ptr<d2d_frame>& frame, const std::unique_ptr<D2D1::Matrix3x2F>& scaleTransform);
void DrawGameObject(const game_object& gameObject, const std::unique_ptr<d2d_frame>& frame);
void DrawLevel(const game_level& level, const d2d_frame& frame);
std::unique_ptr<D2D1::Matrix3x2F> CreateScaleTransform(const winrt::com_ptr<ID2D1HwndRenderTarget>& renderTarget, float screenWidth, float screenHeight);
