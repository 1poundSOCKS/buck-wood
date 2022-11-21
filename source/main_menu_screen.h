#ifndef _main_menu_screen_
#define _main_menu_screen_

#include "framework.h"
#include "control_state.h"
#include "sound_buffers.h"
#include "diagnostics.h"
#include "game_objects.h"
#include "global_state.h"

struct main_menu_control_state
{
  bool quit = false;
  bool startPlay = false;
  bool startLevelEditor = false;
  bool cancelExit = false;
  bool saveChanges = false;
  bool discardChanges = false;
  float renderTargetMouseX = 0;
  float renderTargetMouseY = 0;
};

struct main_menu_screen_state
{
  main_menu_screen_state(const global_state& globalState);

  enum view_state { view_default, view_exit };
  view_state viewState = view_default;

  const global_state& globalState;
  // const d2d_brushes& brushes;
  const render_brushes renderBrushes;
  const dwrite_text_formats& textFormats;

  float renderTargetMouseX = 0, renderTargetMouseY = 0;

  bool starting = true;
  bool running = true;
  bool quit = false;
  bool startPlay = false;
  bool startLevelEdit = false;
  bool saveGameLevelData = false;
  bool checkSaveOnExit = true;
};

void RefreshControlState(main_menu_control_state& screenControlState, const control_state& controlState);
void UpdateScreenState(main_menu_screen_state& screenState, const main_menu_control_state& controlState, const system_timer& timer);
void RenderFrame(const d2d_frame& frame, main_menu_screen_state& screenState);
void UpdateSound(const sound_buffers& soundBuffers, const main_menu_screen_state& gameState);
void FormatDiagnostics(diagnostics_data& diagnosticsData, const main_menu_screen_state& screenState, const main_menu_control_state& controlState);
void UpdateGlobalState(global_state& globalState, const main_menu_screen_state& screenState);

#endif
