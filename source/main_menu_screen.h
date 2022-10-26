#ifndef _main_menu_screen_
#define _main_menu_screen_

#define UNICODE

#include "type_defs.h"
#include "system_timer.h"
#include "d2d_frame.h"
#include "control_state.h"
#include "sound_buffers.h"
#include "diagnostics.h"

struct main_menu_screen_state
{
  main_menu_screen_state();

  bool starting = true;
  bool running = true;
  bool quit = false;
  bool startPlay = false;
  bool startLevelEdit = false;
};

// using game_state_ptr = std::unique_ptr<main_menu_screen_state>;

void RenderFrame(const d2d_frame& frame, main_menu_screen_state& screenState);
void UpdateState(main_menu_screen_state& screenState, const control_state& controlState, const system_timer& timer);
void UpdateSound(const sound_buffers& soundBuffers, const main_menu_screen_state& gameState);
void FormatDiagnostics(diagnostics_data& diagnosticsData, const main_menu_screen_state& screenState, const control_state& controlState, const perf_data& perfData, const system_timer& timer);

#endif
