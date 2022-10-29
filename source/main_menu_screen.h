#ifndef _main_menu_screen_
#define _main_menu_screen_

#define UNICODE

#include "framework/framework.h"
#include "type_defs.h"
#include "d2d_frame.h"
#include "control_state.h"
#include "sound_buffers.h"
#include "diagnostics.h"
#include "game_objects.h"

struct main_menu_screen_state
{
  main_menu_screen_state();

  mouse_cursor mouseCursor;

  bool starting = true;
  bool running = true;
  bool quit = false;
  bool startPlay = false;
  bool startLevelEdit = false;
};

void RenderFrame(const d2d_frame& frame, main_menu_screen_state& screenState);
void UpdateState(main_menu_screen_state& screenState, const control_state& controlState, const system_timer& timer);
void UpdateSound(const sound_buffers& soundBuffers, const main_menu_screen_state& gameState);
void FormatDiagnostics(diagnostics_data& diagnosticsData, const main_menu_screen_state& screenState, const control_state& controlState, const perf_data& perfData, const system_timer& timer);

#endif
