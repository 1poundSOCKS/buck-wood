#define UNICODE

#include <memory>
#include <tchar.h>

#include "d2d_frame.h"
#include "game_state.h"
#include "perf_data.h"

#define RENDER_SCALE_WIDTH    0.5
#define RENDER_SCALE_HEIGHT   0.5

void DoRender(const std::unique_ptr<d2d_frame>&, const std::unique_ptr<game_state>&, const std::unique_ptr<perf_data>&);
void RenderMainScreen(const std::unique_ptr<d2d_frame>& frame, const std::unique_ptr<game_state>& gs, const std::unique_ptr<perf_data>& pd);
void RenderTitleScreen(const std::unique_ptr<d2d_frame>& frame);
void SetTransformAndDrawGameObject(const game_object& gameObject, const std::unique_ptr<d2d_frame>& frame);
void DrawGameObject(const game_object& gameObject, const std::unique_ptr<d2d_frame>& frame);
