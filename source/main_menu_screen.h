#ifndef _main_menu_screen_
#define _main_menu_screen_

#define UNICODE

#include "framework/framework.h"
#include "control_state.h"
#include "sound_buffers.h"
#include "diagnostics.h"
#include "game_objects.h"
#include "global_state.h"

struct main_menu_screen_state
{
  main_menu_screen_state(const global_state& globalState);

  const global_state& globalState;
  const d2d_brushes& brushes;
  const dwrite_text_formats& textFormats;

  mouse_cursor mouseCursor;
  float mouseX = 0, mouseY = 0;

  bool starting = true;
  bool running = true;
  bool quit = false;
  bool startPlay = false;
  bool startLevelEdit = false;
};

D2D1::Matrix3x2F CreateViewTransform(const D2D1_SIZE_F& renderTargetSize, const main_menu_screen_state& screenState);
void RenderFrame(const d2d_frame& frame, main_menu_screen_state& screenState);
void UpdateScreenState(main_menu_screen_state& screenState, const D2D1_SIZE_F& renderTargetSize, const control_state& controlState, const system_timer& timer);
void UpdateSound(const sound_buffers& soundBuffers, const main_menu_screen_state& gameState);
void FormatDiagnostics(diagnostics_data& diagnosticsData, const main_menu_screen_state& screenState, const control_state& controlState, const perf_data& perfData, const system_timer& timer);
void UpdateGlobalState(global_state& globalState, const main_menu_screen_state& screenState);

#endif
